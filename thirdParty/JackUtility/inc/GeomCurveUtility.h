#pragma once
#include "GeometryUtilityDefine.h"
#include "GeometryUtilityOccDefine.h"



class JACKC_GEOM_UT_EXPORT GeomCurveUtility
{
public:
    static Handle(Geom_TrimmedCurve) MakeTrimmedSegment(const gp_Pnt& p1, const gp_Pnt& p2);

    static Handle(Geom_BSplineCurve) MakeBSpline(const std::vector<gp_Pnt>& ctrlPts);

};