#pragma once
#include "GeometryUtilityDefine.h"
#include "GeometryUtilityOccDefine.h"



class JACKC_GEOM_UT_EXPORT GeomBSplineSurfaceUtility
{
public:
    static Handle(Geom_BSplineSurface) CreateBSplineSurfaceFromControlPoints(const std::vector<gp_Pnt>& points);



};