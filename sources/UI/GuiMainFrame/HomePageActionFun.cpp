#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <gp_Pnt.hxx>

#include "GuiMainFrameDefine.h"
#include "HomePageActionFun.h"
#include "GuiMainOccDefines.h"
#include <warning.h>
#include <QMessageBox>
#include <QVector3D>

#pragma warning(push)
#pragma warning(disable:4996)
#include <V3d_Light.hxx>
#pragma warning(pop)


#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <IfcImportDialog.h>
#include <Poly_Triangulation.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pnt.hxx>

#include <ifcpp/geometry/GeometryConverter.h>
#include <IfcProject.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <GeometrySettings.h>
#include <IncludeCarveHeaders.h>
#include <GeomDebugDump.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SolidModelConverter.h>
#include <ifcpp/geometry/FaceConverter.h>

#include <ifcpp/IFC4X3/include/IfcFacetedBrep.h>
#include <ifcpp/IFC4X3/include/IfcExtrudedAreaSolid.h>
#include <ifcpp/IFC4X3/include/IfcSolidModel.h>


#include <QString>
#include <QDebug>
#include "IfcImportUtils.h"
#include "WinParametricBuild.h"

#include "HomePage.h"
#include "WinPointLineRelation.h"
#include "WinPointOnSurface.h"
#include "WinSegmentDistance.h"



HomePageActionFun::HomePageActionFun(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView, QWidget* parent, std::function<void(const QString&)> func)
{
    m_context=context;
    m_v3dViewer=v3dViewer;
    m_v3dView=v3dView;
    m_parent=parent;
    m_outputFunc=std::move(func);
}


Handle(AIS_Shape) HomePageActionFun::GetFirstSelectedShape() const
{
    if (m_context->NbSelected() > 0)
    {
        m_context->InitSelected();
        Handle(AIS_InteractiveObject) selectedObj = m_context->SelectedInteractive();
        Handle(AIS_Shape) shape = Handle(AIS_Shape)::DownCast(selectedObj);
        if (!shape.IsNull())
            return shape;
    }
    return Handle(AIS_Shape)(); // 空
}

//void HomePageActionFun::ClearShape()
//{
//    // 获取所有正在显示的交互对象
//    AIS_ListOfInteractive list;
//    m_context->DisplayedObjects(list);
//
//    std::vector<Handle(AIS_InteractiveObject)> toRemove;
//    for (AIS_ListOfInteractive::Iterator it(list); it.More(); it.Next())
//    {
//        Handle(AIS_InteractiveObject) obj = it.Value();
//        if (!obj.IsNull() && obj->IsKind(STANDARD_TYPE(AIS_Shape)))
//        {
//            toRemove.push_back(obj);
//        }
//    }
//
//    for (const auto& obj : toRemove)
//        m_context->Remove(obj, Standard_False);
//
//    m_context->UpdateCurrentViewer();
//}


//-------------------------test--------------------------
void HomePageActionFun::TestSimpleBuildAndTranslate()
{
    gp_Pnt p1(0, 0, 0);
    gp_Pnt p2(100, 0, 0);
    gp_Pnt p3(100, 50, 0);
    gp_Pnt p4(0, 50, 0);

    TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(p1, p2);
    TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(p2, p3);
    TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(p3, p4);
    TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(p4, p1);
    TopoDS_Wire rectWire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);

    TopoDS_Face rectFace = BRepBuilderAPI_MakeFace(rectWire);
    TopoDS_Shape solidBodyShape = BRepPrimAPI_MakePrism(rectFace, gp_Vec(0, 0, 50));
    //倒圆角
    BRepFilletAPI_MakeFillet filletMaker(solidBodyShape);
    for (TopExp_Explorer exp(solidBodyShape, TopAbs_EDGE); exp.More(); exp.Next())
    {
        filletMaker.Add(5, TopoDS::Edge(exp.Current()));
    }
    TopoDS_Shape filletedSolid = filletMaker.Shape();

    //创建一个圆柱体，用于布尔减法计算, 形成带孔洞的零件
    gp_Ax2 axis(gp_Pnt(50, 25, 0), gp_Dir(0, 0, 1));
    TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(axis, 10, 80).Solid();
    TopoDS_Shape cutShape = BRepAlgoAPI_Cut(filletedSolid, cylinder).Shape();

    //绕Z轴旋转45度， 沿X平移100
    gp_Trsf transform;
    transform.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), 3.141592653589793238 / 4);
    transform.SetTranslation(gp_Vec(100, 0, 0));
    TopoDS_Shape moveeShape = BRepBuilderAPI_Transform(cutShape, transform).Shape();


    Handle(AIS_Shape) aisShape = new AIS_Shape(cutShape);
    aisShape->SetColor(Quantity_NOC_GREEN1);
    aisShape->SetWidth(1.0); // 设置线宽
    m_context->Display(aisShape, Standard_True);


    Handle(AIS_Shape) moveAisShape = new AIS_Shape(moveeShape);
    moveAisShape->SetColor(Quantity_NOC_YELLOW);
    moveAisShape->SetWidth(1.0); // 设置线宽
    m_context->Display(moveAisShape, Standard_True);
}

void HomePageActionFun::TestSimpleTopExp()
{
    gp_Pnt p1(0, 0, 0);
    gp_Pnt p2(100, 0, 0);
    gp_Pnt p3(100, 50, 0);
    gp_Pnt p4(0, 50, 0);

    TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(p1, p2);
    TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(p2, p3);
    TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(p3, p4);
    TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(p4, p1);
    TopoDS_Wire rectWire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);

    TopoDS_Face rectFace = BRepBuilderAPI_MakeFace(rectWire);
    TopoDS_Shape solidBodyShape = BRepPrimAPI_MakePrism(rectFace, gp_Vec(0, 0, 50));
    //倒圆角
    BRepFilletAPI_MakeFillet filletMaker(solidBodyShape);
    for (TopExp_Explorer exp(solidBodyShape, TopAbs_EDGE); exp.More(); exp.Next())
    {
        filletMaker.Add(5, TopoDS::Edge(exp.Current()));
    }
    TopoDS_Shape filletedSolid = filletMaker.Shape();

    //创建一个圆柱体，用于布尔减法计算, 形成带孔洞的零件
    gp_Ax2 axis(gp_Pnt(50, 25, 0), gp_Dir(0, 0, 1));
    TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(axis, 10, 80).Solid();
    TopoDS_Shape cutShape = BRepAlgoAPI_Cut(filletedSolid, cylinder).Shape();

    //绕Z轴旋转45度， 沿X平移100
    gp_Trsf transform;
    transform.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), 3.141592653589793238 / 4);
    transform.SetTranslation(gp_Vec(100, 0, 0));
    TopoDS_Shape moveeShape = BRepBuilderAPI_Transform(cutShape, transform).Shape();

    Handle(AIS_Shape) aisShape = new AIS_Shape(cutShape);
    aisShape->SetColor(Quantity_NOC_GREEN1);
    aisShape->SetWidth(1.0); // 设置线宽
    m_context->Display(aisShape, Standard_True);


    Handle(AIS_Shape) moveAisShape = new AIS_Shape(moveeShape);
    moveAisShape->SetColor(Quantity_NOC_YELLOW);
    moveAisShape->SetWidth(1.0); // 设置线宽
    m_context->Display(moveAisShape, Standard_True);


    TopExp_Explorer exp(cutShape, TopAbs_FACE);
    if (exp.More())
    {
        TopoDS_Face face = TopoDS::Face(exp.Current());

        // 显示原始面（蓝色）
        Handle(AIS_Shape) aisFace = new AIS_Shape(face);
        aisFace->SetColor(Quantity_NOC_BLUE1);
        m_context->Display(aisFace, Standard_True);

        // 获取该面的底层曲面Geom_Surface的句柄
        Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
        if (surface.IsNull()) return;

        std::cout << "surface type is:" << surface->DynamicType() << std::endl;
        Handle(Geom_Plane) plane = Handle(Geom_Plane)::DownCast(surface);

        if (plane.IsNull()) return;
        gp_Pnt origin = plane->Location();
        gp_Dir normal = plane->Axis().Direction();
    }

}

// 创建一个边长100 * 50 * 30长方体
void HomePageActionFun::TestCreateRectangle()
{
    gp_Pnt origin(-200, -80, -70);
    gp_Dir normal(1, 2, 1);
    gp_Ax2 axis(origin, normal);
    TopoDS_Shape box = BRepPrimAPI_MakeBox(axis, 100, 50, 30).Shape();

    Handle(AIS_Shape) aisShape = new AIS_Shape(box);
    aisShape->SetColor(Quantity_NOC_BLUE2);
    aisShape->SetWidth(1.0); // 设置线宽
    m_context->Display(aisShape, Standard_True);
}


//  Test GeomCurve
void  HomePageActionFun::TestCreateSimpleBSplineCurve()
{
    TColgp_Array1OfPnt2d points(1, 4);     // 创建一个从索引1到4的二维点数组.  TColgp_Array1OfPnt2d 是一个封装了gp_Pnt2d类型数组的容器类
    points.SetValue(1, gp_Pnt2d(0, 0));
    points.SetValue(2, gp_Pnt2d(1, 2));
    points.SetValue(3, gp_Pnt2d(2, 3));
    points.SetValue(4, gp_Pnt2d(4, 4));
    // 使用4个控制点的非周期三次二维B样条曲线

    // 2. 节点（Knot）向量（每个 knot 值在 [0, 1] 区间递增）
    TColStd_Array1OfReal knots(1, 2);  // 简单的 2 knot（非均匀 B 样条）例子
    knots.SetValue(1, 0.0);
    knots.SetValue(2, 1.0);

    TColStd_Array1OfInteger mults(1, 2);
    mults.SetValue(1, 4);
    mults.SetValue(2, 4);

    Standard_Integer degress = 3;
    Standard_Boolean preiodic = Standard_False;
    Handle(Geom2d_BSplineCurve) curve2d = new Geom2d_BSplineCurve(points, knots, mults, degress, preiodic);

    //定义一个3D平面
    Handle(Geom_Plane) surface = new Geom_Plane(gp::XOY());
    Handle(Geom_Curve) curve3d = GeomAPI::To3d(curve2d, surface->Pln());

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(curve3d);

    // 显示
    Handle(AIS_Shape) aisCurve = new AIS_Shape(edge);
    m_context->Display(aisCurve, Standard_True);
}


//------------------------View-----------------------
void HomePageActionFun::TestFrontCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Front);
}

void HomePageActionFun::TestBackCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Back);
}

void HomePageActionFun::TestLeftCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Left);
}

void HomePageActionFun::TestRightCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Right);
}

void HomePageActionFun::TestTopCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Top);
}

void HomePageActionFun::TestBottomCoreView()
{
    SetStandardView(m_v3dView, EnumCoreViewControlType::Bottom);
}

void HomePageActionFun::TestEditShape()
{
    ShapeEditDialog dlg(m_parent);
    if (dlg.exec() == QDialog::Accepted)
    {
        ApplyTransformToSelected(dlg.getType(), dlg.getVector(), dlg.getValue(), dlg.getMirrorPlane());
    }
}

void HomePageActionFun::TestBuildBaseMode()
{
    WinParametricBuild* dlg=new WinParametricBuild(m_context,m_v3dViewer,m_v3dView,m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}



void HomePageActionFun::ApplyTransformToSelected( ShapeEditDialog::TransformType type, QVector3D vec, double value, int mirrorPlane)
{
    // 获取当前选中形体
    Handle(AIS_Shape) selectedShape = GetFirstSelectedShape(); 
    if (selectedShape.IsNull())
    {
        QMessageBox::warning(m_parent, "提示", "未选中形体！");
        return;
    }
    TopoDS_Shape shape = selectedShape->Shape();
    gp_Trsf trsf;

    if (type == ShapeEditDialog::Translate)
    {
        if (vec.lengthSquared() < 1e-10 || std::abs(value) < 1e-10) 
            return;

        gp_Vec moveVec(vec.x() * value, vec.y() * value, vec.z() * value);
        trsf.SetTranslation(moveVec);
        m_context->Remove(selectedShape, Standard_True);
    }
    else if (type == ShapeEditDialog::Rotate)
    {
        if (vec.lengthSquared() < 1e-10 || std::abs(value) < 1e-10)
            return;

        gp_Pnt origin(0, 0, 0);
        gp_Dir axisDir(vec.x(), vec.y(), vec.z());
        double angleRad = value * M_PI / 180.0;
        trsf.SetRotation(gp_Ax1(origin, axisDir), angleRad);
        m_context->Remove(selectedShape, Standard_True);
    }
    else if (type == ShapeEditDialog::Mirror)
    {
        gp_Ax2 plane;
        if (mirrorPlane == 0) // YZ面
            trsf.SetMirror(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)));
        else if (mirrorPlane == 1) // XZ面
            trsf.SetMirror(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 1, 0)));
        else // XY面
            trsf.SetMirror(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)));
    }
    m_context->Remove(selectedShape, Standard_True);
    TopoDS_Shape result = BRepBuilderAPI_Transform(shape, trsf, true).Shape();

    Handle(AIS_Shape) newShape = new AIS_Shape(result);
    newShape->SetColor(Quantity_NOC_DARKSEAGREEN1);
    newShape->SetWidth(selectedShape->Width());
    m_context->Display(newShape, Standard_True);
    m_context->UpdateCurrentViewer();

}

void HomePageActionFun::SetStandardView(const Handle(V3d_View)& view, EnumCoreViewControlType v)
{
    gp_Dir dir, up;
    switch (v)
    {
    case EnumCoreViewControlType::Front:   // 正视 XY，Z+
        dir = gp_Dir(0, 0, 1);   up = gp_Dir(0, 1, 0); break;
    case EnumCoreViewControlType::Back:    // 背视 XY，Z-
        dir = gp_Dir(0, 0, -1);  up = gp_Dir(0, 1, 0); break;
    case EnumCoreViewControlType::Left:    // 左视 YZ，X-
        dir = gp_Dir(-1, 0, 0);  up = gp_Dir(0, 0, 1); break;
    case EnumCoreViewControlType::Right:   // 右视 YZ，X+
        dir = gp_Dir(1, 0, 0);   up = gp_Dir(0, 0, 1); break;
    case EnumCoreViewControlType::Top:     // 俯视 ZX，Y-
        dir = gp_Dir(0, -1, 0);  up = gp_Dir(0, 0, 1); break;
    case EnumCoreViewControlType::Bottom:  // 仰视 ZX，Y+
        dir = gp_Dir(0, 1, 0);   up = gp_Dir(0, 0, 1); break;
    case EnumCoreViewControlType::Iso:     // 等轴测（视需要修改）
        dir = gp_Dir(1, 1, 1);   up = gp_Dir(0, 0, 1); break;
    }

    view->SetProj(dir.X(), dir.Y(), dir.Z());
    view->SetUp(up.X(), up.Y(), up.Z());
    view->FitAll();
}




void HomePageActionFun::ImportIfc()
{
    IfcImportDialog dlg(this, m_parent);
    dlg.exec();
}

void HomePageActionFun::ImportIfc(const QString filePath)
{
    std::string ifc_file = filePath.toStdString();
    auto model = std::make_shared<BuildingModel>();
    ReaderSTEP reader;
    try {
        reader.loadModelFromFile(ifc_file, model);
        qDebug() << "IFC文件导入成功";
    }
    catch (const std::exception& e) {
        qDebug() << "导入失败: " << e.what();
        return;
    }

    auto geometry_converter = std::make_shared<GeometryConverter>(model);
    geometry_converter->convertGeometry();

    for (auto& kv : geometry_converter->getShapeInputData()) {
        auto& shapeData = kv.second;
        if (!shapeData) continue;
        // 递归遍历每个 geometric_item 的所有 child_items 和 meshsets
        for (const auto& item : shapeData->m_geometric_items) {
            IfcImportUtils::DisplayAllMeshes(item, m_context);
        }
    }
}


void HomePageActionFun::PointLineRelation()
{
    WinPointLineRelation* dlg = new WinPointLineRelation(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void HomePageActionFun::PointOnFace()
{
    WinPointOnSurface* dlg = new WinPointOnSurface(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void HomePageActionFun::NearestPointsLine2Line()
{
    WinSegmentDistance* dlg = new WinSegmentDistance(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}
