#pragma once
#include "GeometryUtilityOccDefine.h"
#include "GeometryUtilityDefine.h"
#include "ArcBSplineIsectParams.h"




enum class EnumArcBsplineMethod
{
    IntCS,         //GeomAPI_IntCS
    Section,       //BRepAlgoAPI_Section
    EdgeFace,      //IntTools_EdgeFace
    Extrema,       //GeomAPI_ExtremaCurveSurface
};

class JACKC_GEOM_UT_EXPORT ArcBSplineIntersect
{
public:
    bool IntersectArcWithBSplineFace(const TopoDS_Edge& arcEdge, const TopoDS_Face& bsplineFace, std::vector<gp_Pnt>& outPts, EnumArcBsplineMethod method, const ArcBSplineIsectParams& param = {});


private:
    bool Intersect_Arc_BSpline_IntCS(const TopoDS_Edge&, const TopoDS_Face&, std::vector<gp_Pnt>&, const ArcBSplineIsectParams & = {});
    bool Intersect_Arc_BSpline_Section(const TopoDS_Edge&, const TopoDS_Face&, std::vector<gp_Pnt>&, const ArcBSplineIsectParams & = {});
    bool Intersect_Arc_BSpline_EdgeFace(const TopoDS_Edge&, const TopoDS_Face&, std::vector<gp_Pnt>&, const ArcBSplineIsectParams & = {});
    bool Intersect_Arc_BSpline_Extrema(const TopoDS_Edge&, const TopoDS_Face&, std::vector<gp_Pnt>&, const ArcBSplineIsectParams & = {});
    
};