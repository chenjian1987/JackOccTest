#include "TribonAtxImporter.h"

#include <fstream>
#include <regex>
#include <sstream>
#include <cctype>
#include <unordered_map>
#include <optional>

#include <gp_Vec.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>
#include <gp.hxx>

#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Circle.hxx>
#include <GC_MakeCircle.hxx>
#include <Precision.hxx>

// ------------------ 字符串/数值工具 ------------------
static inline std::string ltrim(const std::string& s) {
    size_t i = 0; while (i < s.size() && std::isspace((unsigned char)s[i])) ++i; return s.substr(i);
}
static inline int readLeadingInt(const std::string& s, bool& ok) {
    ok = false; std::string t = ltrim(s); size_t i = 0; while (i < t.size() && std::isdigit((unsigned char)t[i])) ++i;
    if (i == 0) return 0; ok = true; return std::stoi(t.substr(0, i));
}
static std::vector<double> extractAllDoubles(const std::string& s) {
    static const std::regex re(R"(([+-]?\d+(?:\.\d+)?(?:[Ee][+-]?\d+)?))");
    std::vector<double> v; for (std::sregex_iterator it(s.begin(), s.end(), re), e; it != e; ++it) v.push_back(std::stod((*it)[1].str()));
    return v;
}
static std::vector<long long> extractAllInts(const std::string& s) {
    static const std::regex re(R"(([+-]?\d+))");
    std::vector<long long> v; for (std::sregex_iterator it(s.begin(), s.end(), re), e; it != e; ++it) v.push_back(std::stoll((*it)[1].str()));
    return v;
}

// ------------------ 几何辅助 ------------------
gp_Dir TribonAtxImporter::EstimateNormal(const std::vector<gp_Pnt>& loop)
{
    gp_Vec acc(0, 0, 0);
    if (loop.size() < 3) return gp_Dir(0, 0, 1);
    for (size_t i = 0; i < loop.size(); ++i) {
        const gp_Pnt& a = loop[i];
        const gp_Pnt& b = loop[(i + 1) % loop.size()];
        const gp_Pnt& c = loop[(i + 2) % loop.size()];
        acc += gp_Vec(a, b) ^ gp_Vec(b, c);
    }
    if (acc.SquareMagnitude() < 1e-16) acc = gp_Vec(0, 0, 1);
    return gp_Dir(acc);
}

gp_Ax3 TribonAtxImporter::MakeAx3From242(const std::optional<gp_Pnt>& origin240,
    const std::optional<gp_Dir>& zFrom241,
    const std::optional<gp_Dir>& yFrom242,
    const std::vector<gp_Pnt>& loop)
{
    gp_Pnt ori = origin240.value_or(loop.empty() ? gp_Pnt(0, 0, 0) : loop.front());
    gp_Dir z = zFrom241.value_or(EstimateNormal(loop));
    gp_Dir y = yFrom242.value_or(gp_Dir(0, 1, 0));

    gp_Vec xv = gp_Vec(y) ^ gp_Vec(z);
    if (xv.SquareMagnitude() <= 1e-16) {
        gp_Vec tmp = gp_Vec(z) ^ gp_Vec(1, 0, 0);
        if (tmp.SquareMagnitude() <= 1e-16) tmp = gp_Vec(z) ^ gp_Vec(0, 1, 0);
        y = gp_Dir(tmp);
        xv = gp_Vec(y) ^ gp_Vec(z);
    }
    gp_Dir x(xv);
    return gp_Ax3(ori, z, x); // z 为法向
}

// 计算局部 XY 多边形的符号面积（>0: CCW）
static double SignedArea2D(const std::vector<gp_Pnt>& pts) {
    if (pts.size() < 3) return 0.0;
    double a = 0.0;
    for (size_t i = 0; i < pts.size(); ++i) {
        const gp_Pnt& p = pts[i];
        const gp_Pnt& q = pts[(i + 1) % pts.size()];
        a += p.X() * q.Y() - q.X() * p.Y();
    }
    return 0.5 * a;
}

// 在局部 XY 平面造闭合 wire（输入已在局部坐标，Z≈0）
bool TribonAtxImporter::MakePolyWire(const std::vector<gp_Pnt>& ptsIn, TopoDS_Wire& outWire, double tol)
{
    if (ptsIn.size() < 3) return false;

    // 去重
    std::vector<gp_Pnt> pts; pts.reserve(ptsIn.size());
    for (const auto& p : ptsIn) if (pts.empty() || pts.back().Distance(p) > tol) pts.push_back(p);
    if (pts.size() < 3) return false;

    // 收尾闭合
    if (pts.front().Distance(pts.back()) > tol) pts.push_back(pts.front());

    BRepBuilderAPI_MakeWire wmk;
    for (size_t i = 1; i < pts.size(); ++i)
        wmk.Add(BRepBuilderAPI_MakeEdge(pts[i - 1], pts[i]));
    if (!wmk.IsDone()) return false;
    outWire = wmk.Wire();
    return true;
}

// —— 核心：在“局部坐标”完成建模，再整体变换回世界 —— //
TopoDS_Shape TribonAtxImporter::BuildPlateSolid(const AtxPlate& p)
{
    // 1) 轴：优先用解析得到的 frame
    gp_Ax3 ax = p.frame;

    // 局部<->世界变换
    gp_Trsf toWorld; toWorld.SetTransformation(ax);
    gp_Trsf toLocal = toWorld; toLocal.Invert();

    auto worldToLocal2D = [&](const std::vector<gp_Pnt>& src) {
        std::vector<gp_Pnt> dst; dst.reserve(src.size());
        for (const auto& pw : src) {
            gp_Pnt pl = pw.Transformed(toLocal);
            dst.emplace_back(pl.X(), pl.Y(), 0.0); // 压到 Z=0
        }
        return dst;
        };

    // 2) 外环（局部）
    std::vector<gp_Pnt> outerLoc = worldToLocal2D(p.outline);
    // 统一外环为 CCW
    if (SignedArea2D(outerLoc) < 0.0) std::reverse(outerLoc.begin(), outerLoc.end());

    TopoDS_Wire outerW;
    if (!MakePolyWire(outerLoc, outerW)) return TopoDS_Shape();

    // 3) 局部平面面片
    Handle(Geom_Plane) planeLocal = new Geom_Plane(gp::XOY());
    BRepBuilderAPI_MakeFace fmk(planeLocal, outerW, /*OnlyPlane*/ true);

    // 4) 内环（圆孔 & 多边形孔）—— 必须反向（CW）
    for (const auto& h : p.holes) {
        if (h.type == AtxHole::Type::Circle && h.diameter > 0.0) {
            gp_Pnt cLoc = h.center.Transformed(toLocal);
            gp_Ax2 circAx(gp_Pnt(cLoc.X(), cLoc.Y(), 0.0), gp_Dir(0, 0, 1), gp_Dir(1, 0, 0));
            Handle(Geom_Circle) gc = new Geom_Circle(circAx, 0.5 * h.diameter);
            TopoDS_Wire cw = BRepBuilderAPI_MakeWire(BRepBuilderAPI_MakeEdge(gc));
            // 圆孔反向
            cw.Reverse();
            fmk.Add(cw);
        }
        else if (h.type == AtxHole::Type::Polygon && h.poly.size() >= 3) {
            std::vector<gp_Pnt> polyLoc = worldToLocal2D(h.poly);
            // 如果和外环方向相同（>0），就反向
            if (SignedArea2D(polyLoc) > 0.0) std::reverse(polyLoc.begin(), polyLoc.end());
            TopoDS_Wire hw;
            if (MakePolyWire(polyLoc, hw)) fmk.Add(hw);
        }
    }

    if (!fmk.IsDone()) return TopoDS_Shape();
    TopoDS_Face faceLocal = fmk.Face();

    // 5) 局部挤出（沿 +Z 厚度）
    TopoDS_Shape solidLocal = BRepPrimAPI_MakePrism(faceLocal, gp_Vec(0, 0, p.thickness)).Shape();

    // 6) 变回世界
    TopoDS_Shape solidWorld = BRepBuilderAPI_Transform(solidLocal, toWorld).Shape();
    return solidWorld;
}

TopoDS_Shape TribonAtxImporter::BuildShape(const AtxModel& model)
{
    BRep_Builder bb; TopoDS_Compound comp; bb.MakeCompound(comp);
    for (const auto& p : model.plates) {
        if (p.outline.size() < 3 || p.thickness <= 0) continue;
        TopoDS_Shape s = BuildPlateSolid(p);
        if (!s.IsNull()) bb.Add(comp, s);
    }
    return comp;
}

// ------------------ 解析 ------------------
bool TribonAtxImporter::Parse(const std::string& filePath, AtxModel& out)
{
    std::ifstream in(filePath); if (!in) return false;

    std::string line;
    AtxPlate* curPlate = nullptr;
    std::unordered_map<int, int> idToIndex;

    // pending 的 240/241/242
    std::optional<gp_Pnt> pendingOrigin;
    std::optional<gp_Dir> pendingZ, pendingY;

    auto flushPlateFrame = [&](AtxPlate& p) {
        p.frame = MakeAx3From242(pendingOrigin, pendingZ, pendingY, p.outline);
        pendingOrigin.reset(); pendingZ.reset(); pendingY.reset();
        };

    // 当前特征
    enum class FeatKind { None, Circle, Poly };
    FeatKind featKind = FeatKind::None;
    int      featTargetPlateId = -1;
    AtxHole  feat;                 // circle 用 center/diameter；poly 用 poly
    std::vector<gp_Pnt> featPoly;

    auto flushFeatureIfReady = [&]() {
        if (featKind == FeatKind::None) return;
        // 完成特征 frame（若缺失）
        if (featKind == FeatKind::Circle && feat.frame.Direction().IsEqual(gp_Dir(0, 0, 1), Precision::Angular())) {
            // 允许保持默认；真正建模时会用板 Ax3
        }
        auto it = idToIndex.find(featTargetPlateId);
        if (it != idToIndex.end()) {
            if (featKind == FeatKind::Circle) {
                feat.type = AtxHole::Type::Circle;
                out.plates[it->second].holes.push_back(feat);
            }
            else if (featKind == FeatKind::Poly && featPoly.size() >= 3) {
                AtxHole h; h.type = AtxHole::Type::Polygon; h.poly = featPoly; h.frame = feat.frame;
                out.plates[it->second].holes.push_back(std::move(h));
            }
        }
        // 清理
        featKind = FeatKind::None; featTargetPlateId = -1; feat = AtxHole{}; featPoly.clear();
        pendingOrigin.reset(); pendingZ.reset(); pendingY.reset();
        };

    while (std::getline(in, line)) {
        if (line.empty()) continue;
        bool ok = false; int code = readLeadingInt(line, ok); if (!ok) continue;

        std::string rest = ltrim(line);
        size_t pos = 0; while (pos < rest.size() && std::isdigit((unsigned char)rest[pos])) ++pos;
        if (pos < rest.size()) rest = ltrim(rest.substr(pos)); else rest.clear();

        switch (code) {
        case 214: { // 新板：214 <id>
            flushFeatureIfReady();
            out.plates.emplace_back();
            curPlate = &out.plates.back();
            auto ints = extractAllInts(rest);
            if (!ints.empty()) curPlate->id = int(ints[0]);
            idToIndex[curPlate->id] = int(out.plates.size() - 1);
            pendingOrigin.reset(); pendingZ.reset(); pendingY.reset();
        } break;

        case 215: { if (curPlate) curPlate->name = rest; } break;

        case 200: { // 第二个浮点 = 厚度
            if (curPlate) { auto v = extractAllDoubles(rest); if (v.size() >= 2) curPlate->thickness = v[1]; }
        } break;

        case 15: { // 首点 / 孔中心
            auto v = extractAllDoubles(rest);
            if (v.size() >= 3) {
                gp_Pnt p(v[0], v[1], v[2]);
                if (featKind == FeatKind::Circle)      feat.center = p;
                else if (featKind == FeatKind::Poly) { featPoly.clear(); featPoly.push_back(p); }
                else if (curPlate) { curPlate->outline.clear(); curPlate->outline.push_back(p); }
            }
        } break;

        case 16: { // 后续点：取最后三个为 XYZ
            auto v = extractAllDoubles(rest);
            if (v.size() >= 3) {
                gp_Pnt p(v[v.size() - 3], v[v.size() - 2], v[v.size() - 1]);
                if (featKind == FeatKind::Poly)   featPoly.push_back(p);
                else if (curPlate)                 curPlate->outline.push_back(p);
            }
        } break;

        case 240: { auto v = extractAllDoubles(rest); if (v.size() >= 3) pendingOrigin = gp_Pnt(v[0], v[1], v[2]); } break;
        case 241: { auto v = extractAllDoubles(rest); if (v.size() >= 3) { gp_Vec zv(v[0], v[1], v[2]); if (zv.SquareMagnitude() > 1e-12) pendingZ = gp_Dir(zv); } } break;
        case 242: {
            auto v = extractAllDoubles(rest); if (v.size() >= 3) { gp_Vec yv(v[0], v[1], v[2]); if (yv.SquareMagnitude() > 1e-12) pendingY = gp_Dir(yv); }
            if (featKind != FeatKind::None) { // 特征的 240/241/242
                feat.frame = MakeAx3From242(pendingOrigin, pendingZ, pendingY,
                    featKind == FeatKind::Circle ? std::vector<gp_Pnt>{feat.center} : featPoly);
                pendingOrigin.reset(); pendingZ.reset(); pendingY.reset();
            }
            else if (curPlate) {
                flushPlateFrame(*curPlate);
            }
        } break;

                // 特征/分组
        case 71703: { flushFeatureIfReady(); featKind = FeatKind::Circle; } break;   // 圆孔
        case 71750: { flushFeatureIfReady(); featKind = FeatKind::Poly; featPoly.clear(); } break; // 多边形槽

        case 212: { // 例如 D625
            if (featKind == FeatKind::Circle && !rest.empty() && (rest[0] == 'D' || rest[0] == 'd')) {
                std::string num; for (size_t i = 1; i < rest.size(); ++i) { if (std::isdigit((unsigned char)rest[i]) || rest[i] == '.') num.push_back(rest[i]); else break; }
                if (!num.empty()) feat.diameter = std::stod(num);
            }
        } break;

        case 213: { // 213 -<featId> <plateId>
            auto ints = extractAllInts(rest);
            if (ints.size() >= 2) featTargetPlateId = int(ints[1]);
        } break;

        default: break;
        }
    }

    // 收尾
    if (featKind != FeatKind::None) { // 文件以特征结尾
        auto it = idToIndex.find(featTargetPlateId);
        if (it != idToIndex.end()) {
            if (featKind == FeatKind::Circle) { feat.type = AtxHole::Type::Circle; out.plates[it->second].holes.push_back(feat); }
            else if (featKind == FeatKind::Poly && featPoly.size() >= 3) { AtxHole h; h.type = AtxHole::Type::Polygon; h.poly = featPoly; out.plates[it->second].holes.push_back(std::move(h)); }
        }
    }
    if (curPlate) curPlate->frame = MakeAx3From242(std::nullopt, std::nullopt, std::nullopt, curPlate->outline);

    return true;
}

TopoDS_Shape TribonAtxImporter::Import(const std::string& filePath, AtxModel* parsedOut)
{
    AtxModel model;
    if (!Parse(filePath, model)) return TopoDS_Shape();
    if (parsedOut) *parsedOut = model;
    return BuildShape(model);
}
