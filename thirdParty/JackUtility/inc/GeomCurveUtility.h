#pragma once
#include "GeometryUtilityDefine.h"
#include "GeometryUtilityOccDefine.h"



class JACKC_GEOM_UT_EXPORT GeomCurveUtility
{
public:
    static Handle(Geom_TrimmedCurve) MakeTrimmedSegment(const gp_Pnt& p1, const gp_Pnt& p2);

    static Handle(Geom_BSplineCurve) MakeBSpline(const std::vector<gp_Pnt>& ctrlPts);


public:
    inline void UniquePoints(std::vector<gp_Pnt>& pnts, const Standard_Real tol);

    inline void RefineOnSurface(std::vector<gp_Pnt>& pts, const Handle(Geom_Surface)& surf);
};