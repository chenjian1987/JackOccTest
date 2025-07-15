#pragma once
#include<TopoDS_Shape.hxx>
#include<BRepExtrema_DistShapeShape.hxx>
#include<BRepPrimAPI_MakeBox.hxx>

#include<gp_Vec.hxx>
#include<BRepAdaptor_Surface.hxx>
#include<TopoDS.hxx>
#include<gp_Ax2.hxx>
#include<BRepBndLib.hxx>
#include<Bnd_Box.hxx>
#include<Bnd_OBB.hxx>
#include<gp_Pln.hxx>
#include<gp_Dir.hxx>
#include<ProjLib.hxx>
#include<TopExp.hxx>
#include<TopTools_IndexedMapOfShape.hxx>
#include<BRepBuilderAPI_MakeVertex.hxx>
#include<BRep_Tool.hxx>
#include<TopExp_Explorer.hxx>
#include<AIS_InteractiveContext.hxx>
#include<AIS_Shape.hxx>
#include <GeomAPI_ProjectPointOnCurve.hxx>
#include <TopoDS_Face.hxx>
#include <Geom_Plane.hxx>
#include <TopTools_IndexedDataMapOfShapeShape.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Lin.hxx>
#include <IntTools_EdgeEdge.hxx>
#include <gp_Pnt.hxx>
#include <Precision.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <Geom_Line.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <BRepTools.hxx>
#include <Geom_Surface.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <GeomLProp_SLProps.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <TopAbs_State.hxx>
#include <Standard_Real.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <ShapeFix_Shape.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <BRepIntCurveSurface_Inter.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_Shape.hxx>
#include <WNT_Window.hxx>          // 窗口管理
#include <Graphic3d_GraphicDriver.hxx>
#include <Prs3d_ArrowAspect.hxx>


#include <Aspect_Handle.hxx>  // 定义Aspect_Handle类型
#include <WNT_Window.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <GC_MakeSegment.hxx>
#include <AIS_Shape.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <TopoDS_Edge.hxx>
#include <AIS_ViewCube.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Geom_Axis2Placement.hxx>
#include <AIS_Trihedron.hxx>
#include <TopoDS_Wire.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Face.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <TopoDS_Edge.hxx> 
#include <TopoDS.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <Geom2d_BSplineCurve.hxx>


#include <Geom2d_BSplineCurve.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <Geom2d_BSplineCurve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GeomAPI.hxx>
#include <AIS_Point.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <GeomAPI.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Extrema_ExtPC.hxx>
#include <ShapeAnalysis_Curve.hxx>
#include "AIS_TextLabel.hxx"
#include <BRepBuilderAPI_MakeFace.hxx>  // 用于投影计算
#include <Geom_Plane.hxx>            // 用于平面
#include <Geom_BSplineSurface.hxx>   // 用于B样条曲面
#include <GeomAPI_ProjectPointOnSurf.hxx> // 用于投影计算
#include <Geom_CartesianPoint.hxx>   // 用于点
#include <AIS_Shape.hxx>             // 用于绘制形状
#include <AIS_Point.hxx>             // 用于绘制点
#include <Prs3d_LineAspect.hxx>      // 用于绘制线条
#include <BRepPrimAPI_MakeSphere.hxx> 
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>