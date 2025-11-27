#pragma once
#include <string>
#include <vector>
#include <optional>

#include "GuiMainOccDefines.h"




struct AtxHole
{
    enum class Type { Circle, Polygon };
    Type type = Type::Polygon;

    // 圆孔
    gp_Pnt  center;          // 世界坐标
    double  diameter = 0.0;

    // 71750：多边形孔（世界坐标）
    std::vector<gp_Pnt> poly;

    // 特征局部坐标（若为空，建模时用板的 Ax3）
    gp_Ax3  frame;
};

struct AtxPlate
{
    int         id = -1;               // 214 后的编号
    std::string name;
    double      thickness = 0.0;       // mm
    std::vector<gp_Pnt> outline;       // 世界坐标外轮廓
    gp_Ax3      frame;                 // 240/241/242 推得（或回退估计）
    std::vector<AtxHole> holes;        // 71703 + 71750
};

struct AtxModel { std::vector<AtxPlate> plates; };

class TribonAtxImporter
{
public:
    static TopoDS_Shape Import(const std::string& filePath, AtxModel* parsedOut = nullptr);

private:
    static bool         Parse(const std::string& filePath, AtxModel& out);
    static TopoDS_Shape BuildShape(const AtxModel& model);

    // 在局部 XY（Z=0）做闭合线环
    static bool MakePolyWire(const std::vector<gp_Pnt>& pts, TopoDS_Wire& outWire, double tol = 1.0e-6);

    // 在“局部坐标”里造面/内环/挤出，再整体变回世界
    static TopoDS_Shape BuildPlateSolid(const AtxPlate& p);

    static gp_Dir EstimateNormal(const std::vector<gp_Pnt>& loop);
    static gp_Ax3 MakeAx3From242(const std::optional<gp_Pnt>& origin240,
        const std::optional<gp_Dir>& zFrom241,
        const std::optional<gp_Dir>& yFrom242,
        const std::vector<gp_Pnt>& loop);
};