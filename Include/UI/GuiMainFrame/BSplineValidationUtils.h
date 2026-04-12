#pragma once

#include <functional>
#include <vector>

#include <QString>

#include <AIS_InteractiveContext.hxx>
#include <GeomAbs_Shape.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

namespace JackC
{
    /// <summary>
    /// B 样条显示样式：控制曲线、控制多边形、控制点的颜色与线宽。
    /// </summary>
    struct BSplineDisplayStyle
    {
        gp_Vec translation = gp_Vec(0, 0, 0);
        Quantity_Color curveColor = Quantity_Color(0.1, 0.35, 0.90, Quantity_TOC_RGB);
        Quantity_Color polygonColor = Quantity_Color(0.55, 0.55, 0.55, Quantity_TOC_RGB);
        Quantity_Color poleColor = Quantity_Color(0.85, 0.05, 0.05, Quantity_TOC_RGB);

        int curveDisplayMode = 0;
        double curveWidth = 2.0;
        double polygonWidth = 1.0;
    };

    struct BSplineDeBoorLayer
    {
        int level = 0;
        std::vector<gp_Pnt> points;
        std::vector<double> alphas;
    };

    struct BSplinePointEvaluationResult
    {
        bool ok = false;
        QString message;

        double parameter = 0.0;
        int degree = 0;
        int uniqueSpanIndex = -1;   // 基于 unique knots 的区间索引（1-based）
        int fullSpanIndex = -1;     // 基于完整 knot vector 的 span 索引（0-based）
        double spanLeft = 0.0;
        double spanRight = 0.0;

        std::vector<int> activePoleIndices; // OCC pole 索引（1-based）
        std::vector<gp_Pnt> activePoles;
        std::vector<double> expandedKnots;
        std::vector<BSplineDeBoorLayer> layers;

        gp_Pnt occPoint;
        gp_Pnt deBoorPoint;
        double deviation = 0.0;
    };

    class BSplineValidationUtils
    {
    public:
        using OutputFunc = std::function<void(const QString&)>;

    public:
        static QString ContinuityToString(GeomAbs_Shape shape);
        static Quantity_Color MakeColor(double r, double g, double b);
        static QString RealToString(double value, int precision = 3);
        static QString PointToString(const gp_Pnt& point, int precision = 3);

    public:
        static Handle(Geom_BSplineCurve) CreateBSpline3d(
            const std::vector<gp_Pnt>& polesVec,
            const std::vector<double>& knotsVec,
            const std::vector<int>& multsVec,
            int degree,
            bool periodic = false);

        static std::vector<gp_Pnt> GetPoles(const Handle(Geom_BSplineCurve)& curve);
        static int SumMultiplicities(const Handle(Geom_BSplineCurve)& curve);
        static QString BuildCurveSummary(const QString& caseName, const Handle(Geom_BSplineCurve)& curve);

        static TopoDS_Shape MakeControlPolygonShape(const std::vector<gp_Pnt>& poles, const gp_Vec& tr = gp_Vec(0, 0, 0));
        static TopoDS_Shape MakeCurveEdgeShape(const Handle(Geom_BSplineCurve)& curve, const gp_Vec& tr = gp_Vec(0, 0, 0));

        static void DisplayShape(
            const Handle(AIS_InteractiveContext)& context,
            const TopoDS_Shape& shape,
            const Quantity_Color& color,
            int displayMode,
            double width,
            bool updateViewer = false,
            double transparency = 0.0);

        static void DisplayPoles(
            const Handle(AIS_InteractiveContext)& context,
            const std::vector<gp_Pnt>& poles,
            const gp_Vec& tr,
            const Quantity_Color& color);

        static void DisplayCurveCase(
            const Handle(AIS_InteractiveContext)& context,
            const Handle(Geom_BSplineCurve)& curve,
            const BSplineDisplayStyle& style,
            bool updateViewer = false);

    public:
        static double MinDistancePoleToCurveBySampling(
            const Handle(Geom_BSplineCurve)& curve,
            const gp_Pnt& pole,
            int sampleCount = 2000);

        static void LogPoleDistance(
            const OutputFunc& outputFunc,
            const QString& caseName,
            const Handle(Geom_BSplineCurve)& curve);

        static int FindSpanIndex(const Handle(Geom_BSplineCurve)& curve, double u);
        static std::vector<double> ExpandKnots(const Handle(Geom_BSplineCurve)& curve);

        static bool EvaluatePointByDeBoor(
            const Handle(Geom_BSplineCurve)& curve,
            double u,
            BSplinePointEvaluationResult& outResult);

        static QString BuildPointEvaluationSummary(
            const QString& caseName,
            const BSplinePointEvaluationResult& result);

        static void LogPointEvaluationSamplingComparison(
            const OutputFunc& outputFunc,
            const QString& caseName,
            const Handle(Geom_BSplineCurve)& curve,
            int sampleCount = 41);

        static void LogLocalControlDeviation(
            const OutputFunc& outputFunc,
            const Handle(Geom_BSplineCurve)& baseCurve,
            const Handle(Geom_BSplineCurve)& movedCurve);

        static void LogContinuityAtKnot(
            const OutputFunc& outputFunc,
            const QString& caseName,
            const Handle(Geom_BSplineCurve)& curve,
            int knotIndex);

    public:
        static Handle(Geom_BSplineCurve) CopyCurve(const Handle(Geom_BSplineCurve)& curve);
        static Handle(Geom_BSplineCurve) CopyCurveWithMovedPole(
            const Handle(Geom_BSplineCurve)& curve,
            int poleIndex,
            const gp_Vec& delta);

        static QString ValidateOpenBSplineInput(
            const std::vector<gp_Pnt>& polesVec,
            const std::vector<double>& knotsVec,
            const std::vector<int>& multsVec,
            int degree);

        static Handle(Geom_BSplineCurve) CreateOpenBSpline3dChecked(
            const std::vector<gp_Pnt>& polesVec,
            const std::vector<double>& knotsVec,
            const std::vector<int>& multsVec,
            int degree,
            QString& outMsg);
    };
}
