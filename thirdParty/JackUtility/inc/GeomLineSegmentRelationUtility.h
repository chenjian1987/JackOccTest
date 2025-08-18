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
    bool DistanceBSplineUsingExtremaCurveCurve(const gp_Pnt& p1, const gp_Pnt& p2,const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingDistShapeShape(const gp_Pnt& p1, const gp_Pnt& p2,const std::vector<gp_Pnt>& ctrlPts,gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingProjectPointOnCurve(const gp_Pnt& p1, const gp_Pnt& p2, const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);

    bool DistanceBSplineUsingShapeAnalysisProject(const gp_Pnt& p1, const gp_Pnt& p2, const std::vector<gp_Pnt>& ctrlPts, gp_Pnt& nearPt1, gp_Pnt& nearPt2);



};