#include "BSplineValidationUtils.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <Geom_Point.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Geom_BSplineCurve.hxx>

#include <AIS_Point.hxx>
#include <AIS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>

#include <Prs3d_Drawer.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Aspect_TypeOfMarker.hxx>

#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>

#include <gp_Trsf.hxx>

#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>

using namespace JackC;

// 将occ的连续性枚举转换为字符串，方便显示在UI上
QString BSplineValidationUtils::ContinuityToString(GeomAbs_Shape shape)
{
	switch (shape)
	{
	case GeomAbs_C0: return "C0";
	case GeomAbs_G1: return "G1";
	case GeomAbs_C1: return "C1";
	case GeomAbs_G2: return "G2";
	case GeomAbs_C2: return "C2";
	case GeomAbs_C3: return "C3";
	case GeomAbs_CN: return "CN";
	default: return "Unknown";
	}
}

// 创建rgb颜色
Quantity_Color BSplineValidationUtils::MakeColor(double r, double g, double b)
{
	return Quantity_Color(r, g, b, Quantity_TOC_RGB);
}

QString BSplineValidationUtils::RealToString(double value, int pre)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(pre) << value;
	return QString::fromStdString(oss.str());
}

// 创建3d B样条曲线    poles控制点  knots节点
Handle(Geom_BSplineCurve) BSplineValidationUtils::CreateBSpline3d(const std::vector<gp_Pnt>& polesVec, const std::vector<double>& knotsVec, const std::vector<int>& multsVec, int degree, bool periodic)
{
	TColgp_Array1OfPnt poles(1, static_cast<Standard_Integer>(polesVec.size()));
	for (Standard_Integer i = 1; i <= static_cast<Standard_Integer>(polesVec.size()); ++i)
	{
		poles.SetValue(i, polesVec[static_cast<size_t>(i - 1)]);
	}

	TColStd_Array1OfReal knots(1, static_cast<Standard_Integer>(knotsVec.size()));
	for (Standard_Integer i = 1; i <= static_cast<Standard_Integer>(knotsVec.size()); ++i)
	{
		knots.SetValue(i, knotsVec[static_cast<size_t>(i - 1)]);
	}

	TColStd_Array1OfInteger mults(1, static_cast<Standard_Integer>(multsVec.size()));
	for (Standard_Integer i = 1; i <= static_cast<Standard_Integer>(multsVec.size()); ++i)
	{
		mults.SetValue(i, multsVec[static_cast<size_t>(i - 1)]);
	}

	return new Geom_BSplineCurve(poles, knots, mults, degree, periodic);
}

// 获取B样条曲线的所有控制点
std::vector<gp_Pnt> BSplineValidationUtils::GetPoles(const Handle(Geom_BSplineCurve)& curve)
{
	std::vector<gp_Pnt> pts;
	if (curve.IsNull()) return pts;

	for (Standard_Integer i = 1; i <= curve->NbPoles(); ++i)
	{
		pts.push_back(curve->Pole(i));
	}
	return pts;
}

// 计算所有knots multiplicity之和，非周期曲线满足 NbPoles = Sum(Mults) - Degree - 1
int BSplineValidationUtils::SumMultiplicities(const Handle(Geom_BSplineCurve)& curve)
{
	int sum = 0;
	if (curve.IsNull()) return sum;

	for (Standard_Integer i = 1; i <= curve->NbKnots(); ++i)
	{
		sum += curve->Multiplicity(i);
	}
	return sum;
}

QString BSplineValidationUtils::BuildCurveSummary(const QString& caseName, const Handle(Geom_BSplineCurve)& curve)
{
	if (curve.IsNull())
		return QStringLiteral("[%1] curve is null").arg(caseName);

	QString msg;
	msg += QStringLiteral("--------------------------------------------------------\n");
	msg += QStringLiteral("[%1]\n").arg(caseName);
	msg += QStringLiteral("次数 Degree = %1, 控制点 NbPoles = %2, 节点 NbKnots = %3, 连续性 Continuity = %4\n")
		.arg(curve->Degree())
		.arg(curve->NbPoles())
		.arg(curve->NbKnots())
		.arg(ContinuityToString(curve->Continuity()));

	int sumMults = 0;
	msg += QStringLiteral("Knots / Mults: ");
	for (Standard_Integer i = 1; i <= curve->NbKnots(); ++i)
	{
		sumMults += curve->Multiplicity(i);
		msg += QStringLiteral("[%1 | M=%2] ")
			.arg(RealToString(curve->Knot(i), 3))
			.arg(curve->Multiplicity(i));
	}
	msg += QStringLiteral("\n");

	msg += QStringLiteral("Check: Poles = Sum(Mults) - Degree - 1 => %1 = %2 - %3 - 1\n")
		.arg(curve->NbPoles())
		.arg(sumMults)
		.arg(curve->Degree());

	msg += QStringLiteral("Poles: ");
	for (Standard_Integer i = 1; i <= curve->NbPoles(); ++i)
	{
		gp_Pnt p = curve->Pole(i);
		msg += QStringLiteral("P%1(%2,%3,%4) ")
			.arg(i)
			.arg(RealToString(p.X(), 1))
			.arg(RealToString(p.Y(), 1))
			.arg(RealToString(p.Z(), 1));
	}
	msg += QStringLiteral("\n");

	return msg;
}

// 几何显示构造：根据控制点构造控制多边形
TopoDS_Shape BSplineValidationUtils::MakeControlPolygonShape(const std::vector<gp_Pnt>& poles, const gp_Vec& tr)
{
	BRep_Builder builder;
	TopoDS_Compound comp;
	builder.MakeCompound(comp);

	if (poles.size() < 2) return comp;

	for (size_t i = 0; i + 1 < poles.size(); ++i)
	{
		gp_Pnt p1 = poles[i].Translated(tr);
		gp_Pnt p2 = poles[i + 1].Translated(tr);

		BRepBuilderAPI_MakeEdge mkEdge(p1, p2);
		if (!mkEdge.IsDone())
			continue;

		builder.Add(comp, mkEdge.Edge());
	}
	return comp;
}

// 根据B样条构造一条边，用于观察曲线的走向
TopoDS_Shape BSplineValidationUtils::MakeCurveEdgeShape(const Handle(Geom_BSplineCurve)& curve, const gp_Vec& tr)
{
	if (curve.IsNull()) return TopoDS_Shape();

	BRepBuilderAPI_MakeEdge mkEdge(curve);
	if (!mkEdge.IsDone()) return TopoDS_Shape();

	TopoDS_Edge edge = mkEdge.Edge();
	if (tr.Magnitude() < 1.0e-12) return edge;

	gp_Trsf tf;
	tf.SetTranslation(tr);

	BRepBuilderAPI_Transform trsf(edge, tf, Standard_True);
	return trsf.Shape();
}

// 显示一个TopoDS_Shape
void BSplineValidationUtils::DisplayShape(const Handle(AIS_InteractiveContext)& context, const TopoDS_Shape& shape, const Quantity_Color& color, int displayMode, double width, bool updateViewer, double transparency)
{
	if (context.IsNull() || shape.IsNull())
		return;

	Handle(AIS_Shape) ais = new AIS_Shape(shape);
	ais->SetColor(color);
	ais->SetDisplayMode(displayMode);
	ais->SetWidth(width);

	if (transparency > 0.0)
		ais->SetTransparency(transparency);

	context->Display(ais, updateViewer ? Standard_True : Standard_False);
}

// 绘制顶点
void BSplineValidationUtils::DisplayPoles(const Handle(AIS_InteractiveContext)& context, const std::vector<gp_Pnt>& poles, const gp_Vec& tr, const Quantity_Color& color)
{
	if (context.IsNull())
		return;

	for (size_t i = 0; i < poles.size(); ++i)
	{
		// 把控制点平移到目标位置后，包装成occ可显示的几何点对象
		Handle(Geom_Point) pointGeom = new Geom_CartesianPoint(poles[i].Translated(tr));

		// 再把几何点包装为AIS_Point进行显示
		Handle(AIS_Point) pointAis = new AIS_Point(pointGeom);

		// 创建显示属性容器，控制点怎么画、画成什么样
		Handle(Prs3d_Drawer) drawer = new Prs3d_Drawer();

		// 设置点的显示样式
		Handle(Prs3d_PointAspect) pointAspect = new Prs3d_PointAspect(Aspect_TOM_POINT, color, 4.0);

		drawer->SetPointAspect(pointAspect);
		pointAis->SetAttributes(drawer);

		context->Display(pointAis, Standard_False);
	}
}

// 显示一个完整的B样条曲线案例，包括曲线本体（边）、控制多边形（边）和控制点（点），方便观察B样条的整体结构和特征
void BSplineValidationUtils::DisplayCurveCase(const Handle(AIS_InteractiveContext)& context, const Handle(Geom_BSplineCurve)& curve, const BSplineDisplayStyle& style, bool updateViewer)
{
	if (curve.IsNull() || context.IsNull())
		return;

	const std::vector<gp_Pnt> poles = GetPoles(curve);

	// 控制多边形
	TopoDS_Shape polyShape = MakeControlPolygonShape(poles, style.translation);
	DisplayShape(context, polyShape, style.polygonColor, 0, style.polygonWidth, false, 0.0);

	// 曲线
	TopoDS_Shape edgeShape = MakeCurveEdgeShape(curve, style.translation);
	DisplayShape(context, edgeShape, style.curveColor, style.curveDisplayMode, style.curveWidth, false, 0.0);

	// 控制点
	DisplayPoles(context, poles, style.translation, style.poleColor);

	if (updateViewer)
		context->UpdateCurrentViewer();
}

// 通过密集采样计算控制点到曲线的最小距离
double BSplineValidationUtils::MinDistancePoleToCurveBySampling(const Handle(Geom_BSplineCurve)& curve, const gp_Pnt& pole, int sampleCount)
{
	if (curve.IsNull() || sampleCount <= 0) return 0.0;

	// 获取曲线的参数定义域，一般曲线在u0~u1上定义
	const double u0 = curve->FirstParameter();
	const double u1 = curve->LastParameter();

	double minDist = 1.0e100;
	for (int i = 0; i <= sampleCount; ++i)
	{
		double t = static_cast<double>(i) / static_cast<double>(sampleCount);
		double u = u0 + (u1 - u0) * t;

		gp_Pnt pt;
		curve->D0(u, pt);
		minDist = std::min(minDist, pole.Distance(pt));
	}
	return minDist;
}

// 输出每个控制点到曲线的最小距离
void BSplineValidationUtils::LogPoleDistance(const OutputFunc& func, const QString& caseName, const Handle(Geom_BSplineCurve)& curve)
{
	if (!func || curve.IsNull())
		return;

	func(QStringLiteral("------ [%1] Pole 控制点到曲线的最小采样距离 ------").arg(caseName));
	for (Standard_Integer i = 1; i <= curve->NbPoles(); ++i)
	{
		gp_Pnt pole = curve->Pole(i);
		double d = MinDistancePoleToCurveBySampling(curve, pole, 2500);
		func(QStringLiteral("Pole 控制点[%1] ---> min distance = %2")
			.arg(i)
			.arg(RealToString(d, 6)));
	}
}

// 查找参数U落在哪一个knot区间
int BSplineValidationUtils::FindSpanIndex(const Handle(Geom_BSplineCurve)& curve, double u)
{
	if (curve.IsNull()) return -1;

	for (Standard_Integer i = 1; i < curve->NbKnots(); ++i)
	{
		double k1 = curve->Knot(i);
		double k2 = curve->Knot(i + 1);
		if (u >= k1 && u <= k2)
			return static_cast<int>(i);
	}
	return static_cast<int>(curve->NbKnots() - 1);
}

// 对比两条曲线
void BSplineValidationUtils::LogLocalControlDeviation(const OutputFunc& func, const Handle(Geom_BSplineCurve)& baseCurve, const Handle(Geom_BSplineCurve)& movedCurve)
{
	if (!func || baseCurve.IsNull() || movedCurve.IsNull())
		return;

	std::vector<double> spanMax(static_cast<size_t>(baseCurve->NbKnots()) + 1, 0.0);
	const int sampleCount = 3000;
	const double u0 = baseCurve->FirstParameter();
	const double u1 = baseCurve->LastParameter();

	for (int i = 0; i <= sampleCount; ++i)
	{
		double t = static_cast<double>(i) / static_cast<double>(sampleCount);
		double u = u0 + (u1 - u0) * t;

		gp_Pnt p1, p2;
		baseCurve->D0(u, p1);
		movedCurve->D0(u, p2);

		double d = p1.Distance(p2);
		int span = FindSpanIndex(baseCurve, u);

		if (span >= 0)
			spanMax[static_cast<size_t>(span)] = std::max(spanMax[static_cast<size_t>(span)], d);
	}

	func(QStringLiteral("---- 局部控制：各个 knot span 节点区间上的最大偏差 ----"));
	for (Standard_Integer i = 1; i < baseCurve->NbKnots(); ++i)
	{
		func(QStringLiteral("Span [%1, %2] -> max deviation = %3")
			.arg(RealToString(baseCurve->Knot(i), 3))
			.arg(RealToString(baseCurve->Knot(i + 1), 3))
			.arg(RealToString(spanMax[static_cast<size_t>(i)], 4)));
	}
}

// 分析某个knot左右两侧的连续性特性
void BSplineValidationUtils::LogContinuityAtKnot(const OutputFunc& out, const QString& caseName, const Handle(Geom_BSplineCurve)& curve, int knotIndex)
{
	if (!out || curve.IsNull()) return;
	if (knotIndex < 1 || knotIndex > curve->NbKnots()) return;

	const double u = curve->Knot(knotIndex);
	const double eps = 1.0e-6;

	const double firstU = curve->FirstParameter();
	const double lastU = curve->LastParameter();
	const double ul = std::max(firstU, u - eps);
	const double ur = std::min(lastU, u + eps);

	gp_Pnt pl, pr;
	gp_Vec v1l, v1r, v2l, v2r;

	// 左右两侧取极近的参数点，比较点、导数、二阶导
	curve->D1(ul, pl, v1l);
	curve->D1(ur, pr, v1r);
	curve->D2(ul, pl, v1l, v2l);
	curve->D2(ur, pr, v1r, v2r);

	const gp_Vec dv1(v1l.X() - v1r.X(), v1l.Y() - v1r.Y(), v1l.Z() - v1r.Z());
	const gp_Vec dv2(v2l.X() - v2r.X(), v2l.Y() - v2r.Y(), v2l.Z() - v2r.Z());

	out(QStringLiteral("---- [%1] knot = %2, multiplicity = %3 ----")
		.arg(caseName)
		.arg(RealToString(u, 3))
		.arg(curve->Multiplicity(knotIndex)));

	out(QStringLiteral("Expected local continuity = Degree - Mult = %1 - %2 = %3")
		.arg(curve->Degree())
		.arg(curve->Multiplicity(knotIndex))
		.arg(curve->Degree() - curve->Multiplicity(knotIndex)));

	out(QStringLiteral("Point jump  = %1").arg(RealToString(pl.Distance(pr), 8)));
	out(QStringLiteral("D1 jump     = %1").arg(RealToString(dv1.Magnitude(), 8)));
	out(QStringLiteral("D2 jump     = %1").arg(RealToString(dv2.Magnitude(), 8)));
}