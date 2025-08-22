#pragma once
#include "GeometryUtilityOccDefine.h"
#include "GeometryUtilityDefine.h"



class JACKC_GEOM_UT_EXPORT GeomLineSegmentRelationUtility
{
public:
    // 直线段与直线段
    bool DistanceUsingExtremaCurveCurve(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& q1, const gp_Pnt& q2,gp_Pnt& nearPt1,gp_Pnt& nearPt2);

    bool DistanceUsingDistShapeShape(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& q1, const gp_Pnt& q2, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceUsingProjectPointOnCurve(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& q1, const gp_Pnt& q2, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceUsingShapeAnalysisProject(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& q1, const gp_Pnt& q2, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceUsingManual(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& q1, const gp_Pnt& q2, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

public:
    // 直线与B样条曲线计算
    bool DistanceBSplineUsingExtremaCurveCurve(const gp_Pnt& p1, const gp_Pnt& p2,const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingDistShapeShape(const gp_Pnt& p1, const gp_Pnt& p2,const std::vector<gp_Pnt>& ctrlPts,gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingProjectPointOnCurve(const gp_Pnt& p1, const gp_Pnt& p2, const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingShapeAnalysisProject(const gp_Pnt& p1, const gp_Pnt& p2, const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);


public:

    // 使用点到面的距离
    bool IntersectSegmentPlaneUsingDistance(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);


    // 直线与平面的相交点
    bool OmtersectSegementPlaneUsingAnalytic(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);

    // 使用GeomAPI_IntCS 计算线段与平面的交点
    bool IntersectSegmentPlaneUsingIntCS(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);

    // 使用Section
    bool IntersectSegmentPlaneUsingSection(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);

    // 使用IntCurveFace_ShapeIntersector
    bool IntersectSegmentPlaneUsingIntCurveFace(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);

    // 使用IntAna_IntConicQuad
    bool IntersectSegmentPlaneUsingIntAnaIntConicQuad(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pln& plane, gp_Pnt& intersectPt);


public:
    //圆弧线与B样条的相交点



};