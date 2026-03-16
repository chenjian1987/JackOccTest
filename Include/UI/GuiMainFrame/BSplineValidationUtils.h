#pragma once
#include <vector>
#include <functional>

#include <QString>

#include <GeomAbs_Shape.hxx>
#include <Geom_BSplineCurve.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_InteractiveContext.hxx>

namespace JackC
{
	/// <summary>
	/// B样条显示样式 控制曲线颜色、控制多边形颜色，控制点颜色，平移位置等
	/// </summary>
	struct BSplineDisplayStyle
	{
		gp_Vec translation = gp_Vec(0, 0, 0);										// 将整个案例平移到某个位置
		Quantity_Color curveColor = Quantity_Color(0.1, 0.35, 0.90, Quantity_TOC_RGB);		// B样条曲线颜色
		Quantity_Color polygonColor = Quantity_Color(0.55, 0.55, 0.55, Quantity_TOC_RGB);	// 控制多边形颜色
		Quantity_Color poleColor = Quantity_Color(0.85, 0.05, 0.05, Quantity_TOC_RGB);		// 控制点颜色

		int curveDisplayMode = 0;			// 曲线显示模式，0通常等价于线框模式
		double curveWidth = 2.0;			// 曲线线宽
		double polygonWidth = 1.0;			// 控制多边形线宽
	};

	class BSplineValidationUtils
	{
	public:
		using OutputFunc = std::function<void(const QString&)>;

	public:
		/// <summary>
		/// 将occ的连续性枚举转换为字符串，方便显示在UI上
		/// </summary>
		static QString ContinuityToString(GeomAbs_Shape shape);

		/// <summary>
		/// 创建RGB颜色
		/// </summary>
		static Quantity_Color MakeColor(double r, double g, double b);

		/// <summary>
		/// 实数转字符串，方便UI展示
		/// </summary>
		static QString RealToString(double value, int precision = 3);

	public:
		/// <summary>
		/// 根据poles / knots / mults / degree构造3D B样条曲线
		/// </summary>
		/// <param name="polesVec"></param>
		/// <param name="knotsVec">必须严格递增，knotsVec.size()必须等于multsVec.size()，非周期曲线通常满足NbPoles = Sum(Mults) - Degree - 1</param>
		/// <param name="multsVec"></param>
		/// <param name="degree"></param>
		/// <param name="periodic"></param>
		/// <returns></returns>
		static Handle(Geom_BSplineCurve) CreateBSpline3d(const std::vector<gp_Pnt>& polesVec, const std::vector<double>& knotsVec, const std::vector<int>& multsVec, int degree, bool periodic = false);

		/// <summary>
		/// 读取B样条的所有控制点 poles
		/// </summary>
		static std::vector<gp_Pnt> GetPoles(const Handle(Geom_BSplineCurve)& curve);

		/// <summary>
		/// 计算所有 knot multiplicity 之和，非周期曲线通常满足 NbPoles = Sum(Mults) - Degree - 1
		/// </summary>
		static int SumMultiplicities(const Handle(Geom_BSplineCurve)& curve);

		/// <summary>
		/// 生成B样条曲线的总结字符串，包含 degree、NbPoles、NbKnots、knotsVec、multsVec 等信息，方便在UI上显示
		/// </summary>
		static QString BuildCurveSummary(const QString& caseName, const Handle(Geom_BSplineCurve)& curve);

		/// <summary>
		/// 根据控制点构造控制多边形，用于观察控制网格，不参与B样条计算
		/// </summary>
		static TopoDS_Shape MakeControlPolygonShape(const std::vector<gp_Pnt>& poles, const gp_Vec& tr = gp_Vec(0, 0, 0));

		/// <summary>
		/// 根据B样条曲线构造一条边，方便观察曲线的走向，不参与B样条计算
		/// </summary>
		static TopoDS_Shape MakeCurveEdgeShape(const Handle(Geom_BSplineCurve)& curve, const gp_Vec& tr = gp_Vec(0, 0, 0));

		/// <summary>
		/// 显示一个TopoDS_Shape
		/// </summary>
		static void DisplayShape(const Handle(AIS_InteractiveContext)& context, const TopoDS_Shape& shape, const Quantity_Color& color, int displayMode, double width, bool updateViewer = false, double transparency = 0.0);

		/// <summary>
		/// 根据控制点构造AIS_Point对象进行显示，方便观察控制点位置，不参与B样条计算
		/// </summary>
		static void DisplayPoles(const Handle(AIS_InteractiveContext)& context, const std::vector<gp_Pnt>& poles, const gp_Vec& tr, const Quantity_Color& color);

		/// <summary>
		/// 显示一个完整的B样条曲线案例，包括曲线本体（边）、控制多边形（边）和控制点（点），方便观察B样条的整体结构和特征
		/// </summary>
		static void DisplayCurveCase(const Handle(AIS_InteractiveContext)& context, const Handle(Geom_BSplineCurve)& curve, const BSplineDisplayStyle& style, bool updateViewer = false);

	public:
		/// <summary>
		/// 通过密集采样估算某个Pole到曲线的最小距离，用于验证控制点通常不在曲线上
		/// </summary>
		static double MinDistancePoleToCurveBySampling(const Handle(Geom_BSplineCurve)& curve, const gp_Pnt& pole, int sampleCount = 2000);

		/// <summary>
		/// 输出每个控制点 pole 到曲线的最小采样距离
		/// </summary>
		static void LogPoleDistance(const OutputFunc& outputFunc, const QString& caseName, const Handle(Geom_BSplineCurve)& curve);

		/// <summary>
		/// 查找参数u落在哪一个knot span上，返回对应的span index
		/// </summary>
		static int FindSpanIndex(const Handle(Geom_BSplineCurve)& curve, double u);

		/// <summary>
		/// 对比两条曲线的每个knot span上的最大偏差
		/// </summary>
		static void LogLocalControlDeviation(const OutputFunc& outputFunc, const Handle(Geom_BSplineCurve)& baseCurve, const Handle(Geom_BSplineCurve)& movedCurve);

		/// <summary>
		/// 分析某个knot左右两侧的连续性特性，通过对比点位置跳变、一阶跳变、二阶跳变
		/// </summary>
		static void LogContinuityAtKnot(const OutputFunc& outputFunc, const QString& caseName, const Handle(Geom_BSplineCurve)& curve, int knotIndex);
	};
}