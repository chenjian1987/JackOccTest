#pragma once
#include "GeometryUtilityOccDefine.h"
#include "GeometryUtilityDefine.h"
#include "ArcBSplineIsectParams.h"




enum class EnumFreeFaceIntersectFreeFaceMethod
{
    Int_SS,        //GeomAPI_IntSS
    Section,       //BRepAlgoAPI_Section

};

class JACKC_GEOM_UT_EXPORT FreeFaceIntersectFreeFace
{
public:
    std::vector<TopoDS_Edge> IntersectFaceWithFace(const TopoDS_Face& face1, const TopoDS_Face& face2, EnumFreeFaceIntersectFreeFaceMethod method);


private:
    // 使用 BRepAlgoAPI_Section（拓扑截取法） 
    std::vector<TopoDS_Edge> IntersectFaceWithFace_Section(const TopoDS_Face& face1, const TopoDS_Face& face2);

    // 使用 GeomAPI_IntSS + 参数裁剪（几何交线法） 
    std::vector<TopoDS_Edge> IntersectFaceWithFace_IntSS(const TopoDS_Face& face1, const TopoDS_Face& face2);


};