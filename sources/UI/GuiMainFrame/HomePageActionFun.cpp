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

// OCAF 核心头文件
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_AttributeIterator.hxx>

// 属性头文件 (用于存数据)
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TNaming_NamedShape.hxx>
#include <TNaming_Builder.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <TopoDS_Shape.hxx>

#include <BinDrivers.hxx>
#include <QString>
#include <QDebug>
#include "IfcImportUtils.h"
#include "WinParametricBuild.h"

#include "HomePage.h"
#include "WinPointLineRelation.h"
#include "WinPointOnSurface.h"
#include "WinSegmentDistance.h"
#include "WinSegmentBSplineDistance.h"
#include "WinSegmentPlaneIntersection.h"
#include "WinArcBSurfIntersect.h"
#include "WinArcFaceIntersectBSplineFace.h"
#include "Matrix44OperationDialog.h"
#include "BSplineValidationUtils.h"


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
    TopLoc_Location location=m_context->Location(selectedShape);
    const gp_Trsf& pt = location.Transformation();

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
    //m_context->Remove(selectedShape, Standard_True);
    //TopoDS_Shape result = BRepBuilderAPI_Transform(shape, trsf, true).Shape();

    m_context->SetLocation(selectedShape, trsf);

    //Handle(AIS_Shape) newShape = new AIS_Shape(result);
    //newShape->SetColor(Quantity_NOC_DARKSEAGREEN1);
    //newShape->SetWidth(selectedShape->Width());
    //m_context->Display(newShape, Standard_True);
    m_context->UpdateCurrentViewer();

 
    selectedShape->SetColor(Quantity_NOC_DARKSEAGREEN1);
    selectedShape->SetWidth(selectedShape->Width());
    m_context->Display(selectedShape, Standard_True);

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

void HomePageActionFun::NearestPointsLine2BSplineCurve()
{
    WinSegmentBSplineDistance* dlg = new WinSegmentBSplineDistance(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}


void HomePageActionFun::SegPlaneIntersection()
{
    WinSegmentPlaneIntersection* dlg = new WinSegmentPlaneIntersection(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}

void HomePageActionFun::ArcBSplineIntersect()
{
    WinArcBSurfIntersect* dlg = new WinArcBSurfIntersect(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}


void HomePageActionFun::ArcFaceIntersectBSplineFace()
{
    WinArcFaceIntersectBSplineFace* dlg = new WinArcFaceIntersectBSplineFace(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}



// -----OCAF----//
void HomePageActionFun::HelloOCAF()
{
    try 
    {
        /*
            OCAF 类比就是一个只有数据结构的"Excel 表格" 或”文件系统"
              TDF_Label （标签）： 好比文件夹 或 Excel单元格位置（如：A1 B2）它只有地址，没有数据。
              TDF_Attribute （属性）：好比文件夹里面的文件 或 单元格里面的内容（数字、文字、形状） 数据必须挂载在Label上
              TDocStd_Document（文档）：好比整个Excel文件，管理所有的Label和Attribute
        */

        // 创建3个变"量 长 宽  高. 读取这些变量，构建一个3D立方体。 将构建好的立方体形状存回OCAF，保存为.cbf文件
        std::cout << " 初始化 Application 和 Document...." << std::endl;
        //创建应用容器
        Handle(TDocStd_Application) app = new TDocStd_Application;
        //加载二进制文件驱动（否则无法保存文件）
        BinDrivers::DefineFormat(app);

        //新建一个文档，格式为BinOcaf (二进制OCAF)
        Handle(TDocStd_Document) doc;
        app->NewDocument("BinOcaf", doc);

        //检验文档是否创建成功
        if (doc.IsNull())
        {
            std::cerr << "Error: 无法创建文档" << std::endl;
            return;
        }
        //开启一个事务 Transaction   OCAF的修改再Command中进行，方便Undo/Redo
        doc->NewCommand();

        //建立数据结构
        std::cout << "设置参数（长=100，宽=50，高=30） ..." << std::endl;

        //获取根标签(Root Label ，也就是0）
        TDF_Label rootLabel = doc->Main();
        //创建子标签用于存放数据
        // 0:1 用于存放长
        // 0:2 用于存放宽
        // 0:3 用于存放高
        // 0:4 用于存放结果形状
        TDF_Label labelLength = rootLabel.FindChild(1);
        TDF_Label labelWidth = rootLabel.FindChild(2);
        TDF_Label labelHeight = rootLabel.FindChild(3);
        TDF_Label labelShape = rootLabel.FindChild(4);

        //设定具体数值 （使用TDataStd_Real 属性） ，这里Set方法会自动检查，如果该Label已经有这个属性就修改值，没有就创建
        TDataStd_Real::Set(labelLength, 100);
        TDataStd_Real::Set(labelWidth, 50);
        TDataStd_Real::Set(labelHeight, 30);

        //给标签取名字(使用TDataStd_Name属性），方便在查看器查看
        TDataStd_Name::Set(labelHeight, "Length");
        TDataStd_Name::Set(labelWidth, "Width");
        TDataStd_Name::Set(labelHeight, "Height");
        TDataStd_Name::Set(labelShape, "MyShape");

        //从OCAF读取数据，使用Find获取属性指针，然后Get 获取值
        Handle(TDataStd_Real) attrL, attrW, attrH;

        labelLength.FindAttribute(TDataStd_Real::GetID(), attrL);
        labelWidth.FindAttribute(TDataStd_Real::GetID(), attrW);
        labelHeight.FindAttribute(TDataStd_Real::GetID(), attrH);

        double length = attrL->Get();
        double width = attrW->Get();
        double height = attrH->Get();

        std::cout << "当前参数:" << length << " " << width << " " << height << std::endl;
        //建模
        TopoDS_Shape boxShape = BRepPrimAPI_MakeBox(length, width, height).Shape();

        //将Shape数据存回OCAF
        //TNaming_Builder 将TopoDS_Shape转换为TNaming_NamedShape属性的工具
        TNaming_Builder builder(labelShape);
        builder.Generated(boxShape);   //将形状挂载到labelShape(0:4)上

        std::cout << " box 已经构建并存储到Label 0:4上" << std::endl;

        //提交事务
        doc->CommitCommand();

        //保存到磁盘
        PCDM_StoreStatus status = app->SaveAs(doc, "TestBox.cbf");

        app->Close(doc);
        return ;
    }
    catch (Standard_Failure& e)
    {
        std::cerr << "Exception occurred: " << e.GetMessageString() << std::endl;
        return ;
    }

}


// -----坐标系转换----//
void HomePageActionFun::TestMatrixTranslate()
{
    Matrix44OperationDialog* dlg = new Matrix44OperationDialog(m_context, m_v3dViewer, m_v3dView, m_parent, m_outputFunc);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}


// -----Test BRepBuilderAPI_Sewing----//
void HomePageActionFun::TestSimpleSewing()
{
    m_outputFunc(QString::fromLocal8Bit("=== 开始缝合对比实验 (左侧失败 vs 右侧成功) ==="));

    // 1. 准备参数
    double size = 50.0;     // 立方体半长
    double gapSize = 5.0;   // 【故意设置大一点】5.0mm 间隙，让视觉效果更明显！

    // 2. 创建几何体 (顶面悬空)
    std::vector<TopoDS_Face> faces;
    // 底面和四周 (正常)
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(0, 0, -size), gp_Dir(0, 0, 1)), -size, size, -size, size)); // Bottom
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(0, size, 0), gp_Dir(0, 1, 0)), -size, size, -size, size));  // Front
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(0, -size, 0), gp_Dir(0, 1, 0)), -size, size, -size, size)); // Back
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(size, 0, 0), gp_Dir(1, 0, 0)), -size, size, -size, size));   // Right
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(-size, 0, 0), gp_Dir(1, 0, 0)), -size, size, -size, size));   // Left

    // 顶面 (悬空 gapSize = 5.0mm)
    faces.push_back(BRepBuilderAPI_MakeFace(gp_Pln(gp_Pnt(0, 0, size + gapSize), gp_Dir(0, 0, 1)), -size, size, -size, size));

    // 构建原始 Compound
    TopoDS_Compound rawShape;
    BRep_Builder builder;
    builder.MakeCompound(rawShape);
    for (const auto& f : faces) builder.Add(rawShape, f);

    m_outputFunc(QString::fromLocal8Bit("原始模型：顶面间隙 %1 mm").arg(gapSize));

    // 场景 A：缝合失败 (容差太小) -> 放在原点
    double smallTol = 0.1;
    m_outputFunc(QString::fromLocal8Bit("--- 生成左侧模型 (失败)... 容差 %1 (小于间隙)").arg(smallTol));
    TopoDS_Shape shapeFail = SewMeshFaces(rawShape, smallTol);

    // 分析结果 A
    InspectModel("左侧-失败模型", shapeFail);

    // 场景 B：缝合成功 (容差够大) -> 向右平移 200mm
    double bigTol = gapSize + 1.0; // 容差设为 6.0mm，大于 5.0mm
    m_outputFunc(QString::fromLocal8Bit("--- 生成右侧模型 (成功)... 容差 %1 (大于间隙)").arg(bigTol));
    TopoDS_Shape shapeSuccess = SewMeshFaces(rawShape, bigTol);

    // 【关键步骤】将成功模型向右平移 200mm，防止重叠
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(200, 0, 0)); // 向 X 轴正方向移动 200
    TopoDS_Shape shapeSuccessMoved = BRepBuilderAPI_Transform(shapeSuccess, trsf).Shape();

    // 分析结果 B (注意：平移不改变拓扑闭合性，分析 shapeSuccess 即可)
    InspectModel("右侧-成功模型", shapeSuccess);

    // 三维显示 (Visualize)
    m_context->RemoveAll(Standard_True); // 清空屏幕

    // --- 显示 A (失败，红色) ---
    if (!shapeFail.IsNull()) {
        Handle(AIS_Shape) aisFail = new AIS_Shape(shapeFail);
        aisFail->SetColor(Quantity_NOC_RED);      // 红色代表警告/失败
        aisFail->SetTransparency(0.4);            // 透明一点，方便看里面的缝隙
        m_context->Display(aisFail, Standard_False); // False表示先不刷新

        // 高亮显示漏洞边界 (用黄色粗线)
        ShapeAnalysis_FreeBounds safb(shapeFail, 1.0e-6);
        const TopoDS_Compound& openWires = safb.GetOpenWires();
        if (!openWires.IsNull()) {
            Handle(AIS_Shape) aisOpenEdges = new AIS_Shape(openWires);
            aisOpenEdges->SetColor(Quantity_NOC_YELLOW); // 黄色高亮漏洞
            aisOpenEdges->SetWidth(3.0);                 // 线宽加粗
            m_context->Display(aisOpenEdges, Standard_False);
        }
    }

    // --- 显示 B (成功，金色) ---
    if (!shapeSuccessMoved.IsNull()) {
        Handle(AIS_Shape) aisSuccess = new AIS_Shape(shapeSuccessMoved);
        aisSuccess->SetColor(Quantity_NOC_GOLD);  // 金色代表完美修复
        aisSuccess->SetDisplayMode(AIS_Shaded);   // 实体显示 (只有闭合模型才能完美Shaded)
        m_context->Display(aisSuccess, Standard_True); // True表示立即刷新
    }

    m_v3dView->FitAll();

    m_outputFunc(QString::fromLocal8Bit("\n对比完成：\n  <左边(红色)>：缝合失败，依然有漏洞。\n  <右边(金色)>：缝合成功，顶面被拉伸闭合。"));
}

// ---------缝合 Sewing-----------------//
TopoDS_Shape HomePageActionFun::SewMeshFaces(const TopoDS_Shape& rawFaces, double tolerance)
{
    if (rawFaces.IsNull()) return TopoDS_Shape();

    try
    {
        BRepBuilderAPI_Sewing sewingTool;
        sewingTool.Init(tolerance);
        sewingTool.SetNonManifoldMode(Standard_False);  // 禁用非流形
        sewingTool.Add(rawFaces);
        sewingTool.Perform();

        TopoDS_Shape ans = sewingTool.SewedShape();
        if (ans.IsNull())
            return rawFaces;

        return ans;
    }
    catch (...)
    {
        m_outputFunc(QString::fromLocal8Bit("[Error] SPDGeometryBuilder::SewMeshFaces crashed."));
        return rawFaces;
    }
}


void HomePageActionFun::InspectModel(const std::string& name, const TopoDS_Shape& shape)
{
    m_outputFunc("========================================");

    // 使用 QString::arg 组合字符串
    m_outputFunc(QString::fromLocal8Bit("检查模型状态: [%1]"));

    // 统计物理边的数量
    TopTools_IndexedMapOfShape edgeMap;
    TopExp_Explorer exp(shape, TopAbs_EDGE);
    while (exp.More()) {
        edgeMap.Add(exp.Current());
        exp.Next();
    }
    int totalEdges = edgeMap.Extent();

    // 输出总边数
    m_outputFunc(QString::fromLocal8Bit("  -> 总边数 (Total Edges): %1").arg(totalEdges));

    // 使用 ShapeAnalysis_FreeBounds 类直接分析 Shape
    ShapeAnalysis_FreeBounds safb(shape, 1.0e-6);
    const TopoDS_Compound& openWires = safb.GetOpenWires();

    // 统计漏洞边的数量
    int freeEdgeCount = 0;
    TopExp_Explorer expFree(openWires, TopAbs_EDGE);
    while (expFree.More())
    {
        freeEdgeCount++;
        expFree.Next();
    }

    if (freeEdgeCount > 0)
    {
        m_outputFunc(QString::fromLocal8Bit("  -> 状态: [开放/有漏洞] (OPEN SHELL)"));
        m_outputFunc(QString::fromLocal8Bit("  -> 发现漏洞边界边数: %1").arg(freeEdgeCount));
    }
    else
    {
        m_outputFunc(QString::fromLocal8Bit("  -> 状态: [封闭/水密] (WATERTIGHT)"));
        m_outputFunc(QString::fromLocal8Bit("  -> 这是一个完美的实体外壳，可以进行布尔运算。"));
    }
    m_outputFunc("========================================");
}


// ---------简化 ShapeUpgrade_UnifySameDomain-----------------//
void HomePageActionFun::TestSimpleShapeUpgrade()
{
    m_outputFunc(QString::fromLocal8Bit("=== 开始 UnifySameDomain 对比实验 (左侧缝合 vs 右侧缝合+简化) ==="));

    // 1. 创建两个相邻且完全共面的正方形面片
    // 面 1：X 从 0 到 50，Y 从 0 到 50
    TopoDS_Face face1 = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), 0, 50, 0, 50);
    // 面 2：X 从 50 到 100，Y 从 0 到 50 (与面1在 X=50 处相邻)
    TopoDS_Face face2 = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), 50, 100, 0, 50);

    // 将它们放入一个 Compound 中
    TopoDS_Compound rawCompound;
    BRep_Builder builder;
    builder.MakeCompound(rawCompound);
    builder.Add(rawCompound, face1);
    builder.Add(rawCompound, face2);

    // 2. 第一步：必须先缝合 (Sewing)
    // 因为独立的两个面，哪怕紧挨着，在拓扑上也是断开的。Unify 需要处理的是连通的拓扑。
    BRepBuilderAPI_Sewing sewer(1e-3);
    sewer.Add(rawCompound);
    sewer.Perform();
    TopoDS_Shape sewedShape = sewer.SewedShape();

    // 统计左侧缝合后模型的面数和边数
    TopTools_IndexedMapOfShape sewedFaces, sewedEdges;
    TopExp::MapShapes(sewedShape, TopAbs_FACE, sewedFaces);
    TopExp::MapShapes(sewedShape, TopAbs_EDGE, sewedEdges);

    m_outputFunc(QString::fromLocal8Bit("--- 左侧模型 (仅Sewing缝合) ---"));
    m_outputFunc(QString::fromLocal8Bit("  -> 面数 (Faces): %1").arg(sewedFaces.Extent())); // 预期：2 个面
    m_outputFunc(QString::fromLocal8Bit("  -> 边数 (Edges): %1").arg(sewedEdges.Extent())); // 预期：7 条边 (外围6条 + 中间1条共享边)

    // 3. 第二步：使用 ShapeUpgrade_UnifySameDomain 进行简化
    // 开启合并边(true)、合并面(true)、合并B样条(true)
    ShapeUpgrade_UnifySameDomain unifier(sewedShape, true, true, true);
    unifier.Build();
    TopoDS_Shape unifiedShape = unifier.Shape();

    // 统计右侧简化后模型的面数和边数
    TopTools_IndexedMapOfShape unifiedFaces, unifiedEdges;
    TopExp::MapShapes(unifiedShape, TopAbs_FACE, unifiedFaces);
    TopExp::MapShapes(unifiedShape, TopAbs_EDGE, unifiedEdges);

    m_outputFunc(QString::fromLocal8Bit("--- 右侧模型 (Sewing + Unify简化) ---"));
    m_outputFunc(QString::fromLocal8Bit("  -> 面数 (Faces): %1").arg(unifiedFaces.Extent())); // 预期：1 个面
    m_outputFunc(QString::fromLocal8Bit("  -> 边数 (Edges): %1").arg(unifiedEdges.Extent())); // 预期：4 条边 (只有外围4条边，中间线被抹除了)

    // 4. 将简化后的模型向右平移 120mm，方便对比
    gp_Trsf trsf;
    trsf.SetTranslation(gp_Vec(120, 0, 0));
    TopoDS_Shape unifiedShapeMoved = BRepBuilderAPI_Transform(unifiedShape, trsf).Shape();

    // 5. 三维显示 (Visualize)
    m_context->RemoveAll(Standard_True); // 清空屏幕

    // --- 显示左侧 (仅缝合，蓝色) ---
    Handle(AIS_Shape) aisSewed = new AIS_Shape(sewedShape);
    aisSewed->SetColor(Quantity_NOC_BLUE4);
    // 开启线框显示模式，为了清晰地看到中间那条缝合线
    aisSewed->SetDisplayMode(AIS_WireFrame);
    aisSewed->SetWidth(2.0);
    m_context->Display(aisSewed, Standard_False);

    // --- 显示右侧 (简化后，绿色) ---
    Handle(AIS_Shape) aisUnified = new AIS_Shape(unifiedShapeMoved);
    aisUnified->SetColor(Quantity_NOC_GREEN);
    // 同样开启线框显示模式，验证中间的线是否消失了
    aisUnified->SetDisplayMode(AIS_WireFrame);
    aisUnified->SetWidth(2.0);
    m_context->Display(aisUnified, Standard_True);

    m_v3dView->FitAll();
    m_outputFunc(QString::fromLocal8Bit("对比完成：观察屏幕，左侧中间有线，右侧变成了干净的一个大面！"));
    m_outputFunc("==================================================================");
}



// ---------BSpline----------------//
void HomePageActionFun::BsplineCurveTest1()
{
    m_context->RemoveAll(Standard_False);
    m_outputFunc(QStringLiteral("================================================"));
    m_outputFunc(QStringLiteral("图形规则：曲线--彩色线；控制多边形---灰色折线；控制点---红色"));
    m_outputFunc(QStringLiteral("单段三次案例"));

    //控制点
    std::vector<gp_Pnt> poles =
    {
        gp_Pnt(0,   0, 0),
        gp_Pnt(35,  90, 0),
        gp_Pnt(105, -90, 0),
        gp_Pnt(140,  0, 0)
    };
    //节点
    std::vector<double> knots = { 0,1 };
    //非周期三次B样条，每个节点的重数为4（degree + 1），表示曲线在起点和终点处都经过控制点
    std::vector<int> mults = { 4,4 };
    Handle(Geom_BSplineCurve) bspline = BSplineValidationUtils::CreateBSpline3d(poles, knots, mults, 3, Standard_False);

    BSplineDisplayStyle style;
    style.curveColor = BSplineValidationUtils::MakeColor(0.95, 0.55, 0.05);     //曲线颜色（橙色）
    style.polygonColor = BSplineValidationUtils::MakeColor(0.55, 0.55, 0.55);   //控制多边形颜色（灰色）
    style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);      //控制点颜色（红色）

    BSplineValidationUtils::DisplayCurveCase(m_context, bspline, style);

    m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("Case1-多段三次B样条"), bspline));
    m_outputFunc(QStringLiteral("说明：这是文档主案例。它有多个 interior knots，更容易看出局部控制和连续性。"));
    BSplineValidationUtils::LogPoleDistance(m_outputFunc, QStringLiteral("Case1-多段三次B样条"), bspline);
}

/// 节点重数与连续性对比验证
void HomePageActionFun::BsplineCurveKnotMultCompareTest()
{
    BSplineContinuityCompareTest();
}

void HomePageActionFun::BSplineContinuityCompareTest()
{
	m_context->RemoveAll(Standard_False);
    // 三组案例统一使用三个唯一节点值，只修改中间节点重数
    std::vector<double> knots = { 0, 1, 2 };

    // -------------------- Case1：内部重数 = 1，对应 C2 --------------------
    {
        std::vector<gp_Pnt> poles =
        {
            gp_Pnt(0,   0,   0),
            gp_Pnt(40, 110,  0),
            gp_Pnt(90, 110,  0),
            gp_Pnt(140,-70,  0),
            gp_Pnt(190,-70,  0)
        };
        std::vector<int> mults = { 4, 1, 4 };

        QString msg;
        Handle(Geom_BSplineCurve) curve = BSplineValidationUtils::CreateOpenBSpline3dChecked(poles, knots, mults, 3, msg);
        m_outputFunc(QStringLiteral("[Case1-C2] 构造结果：%1").arg(msg));

        if (!curve.IsNull())
        {
            BSplineDisplayStyle style;
            style.translation = gp_Vec(0, 0, 0);
            style.curveColor = BSplineValidationUtils::MakeColor(0.10, 0.45, 0.90);
            style.polygonColor = BSplineValidationUtils::MakeColor(0.55, 0.55, 0.55);
            style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);

            BSplineValidationUtils::DisplayCurveCase(m_context, curve, style, false);
            m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("Case1-C2"), curve));

            // 这里knotIndex = 2，表示检查中间节点 u = 1 的连续性
            BSplineValidationUtils::LogContinuityAtKnot(m_outputFunc, QStringLiteral("Case1-C2"), curve, 2);
        }
    }

    // -------------------- Case2：内部重数 = 2，对应 C1 --------------------
    {
        std::vector<gp_Pnt> poles =
        {
            gp_Pnt(0,   0,   0),
            gp_Pnt(30, 110,  0),
            gp_Pnt(65, 110,  0),
            gp_Pnt(100,  20, 0),
            gp_Pnt(145,-110, 0),
            gp_Pnt(190,-110, 0)
        };
        std::vector<int> mults = { 4, 2, 4 };

        QString msg;
        Handle(Geom_BSplineCurve) curve = BSplineValidationUtils::CreateOpenBSpline3dChecked(poles, knots, mults, 3, msg);
        m_outputFunc(QStringLiteral("[Case2-C1] 构造结果：%1").arg(msg));

        if (!curve.IsNull())
        {
            BSplineDisplayStyle style;
            style.translation = gp_Vec(220, 0, 0);
            style.curveColor = BSplineValidationUtils::MakeColor(0.10, 0.70, 0.20);
            style.polygonColor = BSplineValidationUtils::MakeColor(0.55, 0.55, 0.55);
            style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);

            BSplineValidationUtils::DisplayCurveCase(m_context, curve, style, false);
            m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("Case2-C1"), curve));
            BSplineValidationUtils::LogContinuityAtKnot(m_outputFunc, QStringLiteral("Case2-C1"), curve, 2);
        }
    }

    // -------------------- Case3：内部重数 = 3，对应 C0 --------------------
    {
        std::vector<gp_Pnt> poles =
        {
            gp_Pnt(0,   0,   0),
            gp_Pnt(30, 110,  0),
            gp_Pnt(65, 110,  0),
            gp_Pnt(100,  20, 0),
            gp_Pnt(140, 130, 0),
            gp_Pnt(175,-100, 0),
            gp_Pnt(205, -20, 0)
        };
        std::vector<int> mults = { 4, 3, 4 };

        QString msg;
        Handle(Geom_BSplineCurve) curve = BSplineValidationUtils::CreateOpenBSpline3dChecked(poles, knots, mults, 3, msg);
        m_outputFunc(QStringLiteral("[Case3-C0] 构造结果：%1").arg(msg));

        if (!curve.IsNull())
        {
            BSplineDisplayStyle style;
            style.translation = gp_Vec(460, 0, 0);
            style.curveColor = BSplineValidationUtils::MakeColor(0.88, 0.25, 0.15);
            style.polygonColor = BSplineValidationUtils::MakeColor(0.55, 0.55, 0.55);
            style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);

            BSplineValidationUtils::DisplayCurveCase(m_context, curve, style, false);
            m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("Case3-C0"), curve));
            BSplineValidationUtils::LogContinuityAtKnot(m_outputFunc, QStringLiteral("Case3-C0"), curve, 2);
        }
    }

    m_context->UpdateCurrentViewer();
    m_v3dView->FitAll();
    m_outputFunc(QStringLiteral("============================================================"));
    m_outputFunc(QStringLiteral("结论：对于三次B样条，内部节点重数从1增加到3时，连续性从C2降低到C0。"));
    m_outputFunc(QStringLiteral("============================================================"));
}

void HomePageActionFun::BsplineCurveLocalControlCompareTest()
{
    m_context->RemoveAll(Standard_False);

    m_outputFunc(QStringLiteral("============================================================"));
    m_outputFunc(QStringLiteral("B样条局部控制对比验证开始"));
    m_outputFunc(QStringLiteral("目标：保持 knots / mults / degree 不变，只移动一个控制点，观察曲线局部变化"));
    m_outputFunc(QStringLiteral("============================================================"));

    // 这里使用 10 个控制点、7 个 span，使局部控制现象更明显
    std::vector<gp_Pnt> poles =
    {
        gp_Pnt(0,    0,   0),
        gp_Pnt(30,   60,  0),
        gp_Pnt(60,  -40,  0),
        gp_Pnt(95,   85,  0),
        gp_Pnt(130,  20,  0),
        gp_Pnt(165, -70,  0),
        gp_Pnt(200,  50,  0),
        gp_Pnt(235, -20,  0),
        gp_Pnt(270,  60,  0),
        gp_Pnt(300,   0,  0)
    };

    // 唯一节点值：0 ~ 7，共 8 个唯一节点
    std::vector<double> knots = { 0, 1, 2, 3, 4, 5, 6, 7 };

    // 三次 B 样条，两端夹持，中间节点重数均为 1
    // Sum(Mults) = 4 + 1 + 1 + 1 + 1 + 1 + 1 + 4 = 14
    // NbPoles = 14 - 3 - 1 = 10，刚好匹配
    std::vector<int> mults = { 4, 1, 1, 1, 1, 1, 1, 4 };

    QString msg;
    Handle(Geom_BSplineCurve) baseCurve =
        BSplineValidationUtils::CreateOpenBSpline3dChecked(poles, knots, mults, 3, msg);

    m_outputFunc(QStringLiteral("[BaseCurve] 构造结果：%1").arg(msg));
    if (baseCurve.IsNull())
        return;

    m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("BaseCurve-原始曲线"), baseCurve));

    // 这里选择第 6 个控制点，便于观察中部区域的局部变化
    const int movedPoleIndex = 6;
    const gp_Vec delta(0, 90, 0);

    Handle(Geom_BSplineCurve) movedCurve =
        BSplineValidationUtils::CopyCurveWithMovedPole(baseCurve, movedPoleIndex, delta);

    if (movedCurve.IsNull())
    {
        m_outputFunc(QStringLiteral("[MovedCurve] 构造失败"));
        return;
    }

    m_outputFunc(QStringLiteral("移动控制点编号：Pole[%1]").arg(movedPoleIndex));
    m_outputFunc(QStringLiteral("位移向量：(%1, %2, %3)")
        .arg(delta.X())
        .arg(delta.Y())
        .arg(delta.Z()));

    m_outputFunc(BSplineValidationUtils::BuildCurveSummary(QStringLiteral("MovedCurve-移动控制点后"), movedCurve));

    // 显示原始曲线 
    {
        BSplineDisplayStyle style;
        style.translation = gp_Vec(0, 0, 0);
        style.curveColor = BSplineValidationUtils::MakeColor(0.10, 0.45, 0.90);   // 蓝色：原始曲线
        style.polygonColor = BSplineValidationUtils::MakeColor(0.60, 0.60, 0.60); // 灰色：原始控制多边形
        style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);    // 红色：原始控制点
        style.curveWidth = 2.0;
        style.polygonWidth = 1.0;

        BSplineValidationUtils::DisplayCurveCase(m_context, baseCurve, style, false);
    }

    //  显示修改后的曲线 
    {
        BSplineDisplayStyle style;
        style.translation = gp_Vec(0, 0, 0);
        style.curveColor = BSplineValidationUtils::MakeColor(0.90, 0.30, 0.15);   // 红橙色：修改后曲线
        style.polygonColor = BSplineValidationUtils::MakeColor(0.25, 0.25, 0.25); // 深灰：修改后控制多边形
        style.poleColor = BSplineValidationUtils::MakeColor(0.10, 0.70, 0.20);    // 绿色：修改后控制点
        style.curveWidth = 2.0;
        style.polygonWidth = 1.0;

        BSplineValidationUtils::DisplayCurveCase(m_context, movedCurve, style, false);
    }

    m_context->UpdateCurrentViewer();
    m_v3dView->FitAll();
    BSplineValidationUtils::LogLocalControlDeviation(m_outputFunc, baseCurve, movedCurve);

    m_outputFunc(QStringLiteral("============================================================"));
    m_outputFunc(QStringLiteral("观察结论：移动单个控制点后，曲线变化主要集中在其影响区间，而不是整条曲线均匀变化。"));
    m_outputFunc(QStringLiteral("============================================================"));
}


//  B样条数据合法性验证
void HomePageActionFun::BsplineDataValidityCompareTest()
{
    m_context->RemoveAll(Standard_False);

    m_outputFunc(QStringLiteral("============================================================"));
    m_outputFunc(QStringLiteral("B样条数据合法性验证开始"));
    m_outputFunc(QStringLiteral("目标：合法输入可以正常构造；非法输入在进入 OCC 前被预校验拦截"));
    m_outputFunc(QStringLiteral("注意：本实验只针对非周期 B 样条"));
    m_outputFunc(QStringLiteral("============================================================"));

    //case1 合法输入
    std::vector<gp_Pnt> poles1 = 
    {
        gp_Pnt(0,   0, 0),
        gp_Pnt(30,  60, 0),
        gp_Pnt(60, -30, 0),
        gp_Pnt(90,  80, 0),
        gp_Pnt(120,  -20, 0),
        gp_Pnt(150,  50, 0),
		gp_Pnt(180,  0, 0)
	};
    std::vector<double> knots1 = { 0,1,2,3,4 };
	std::vector<int> mults1 = { 4, 1, 1, 1, 4 };

    RunBsplineValidityTest(QStringLiteral("Case1-合法输入"), poles1, knots1, mults1, 3, true, gp_Vec(0, 0, 0));
    
    //case2 非法输入：控制点数量不匹配
    std::vector<gp_Pnt> poles2=
    {
        gp_Pnt(0,   0, 0),
        gp_Pnt(30,  60, 0),
        gp_Pnt(60, -30, 0),
        gp_Pnt(90,  80, 0),
        gp_Pnt(120,  -20, 0),
        gp_Pnt(150,  50, 0)
    };
    std::vector<double> knots2 = { 0,1,2,3,4 };
    std::vector<int> mults2 = { 4, 1, 1, 1, 4 };
    RunBsplineValidityTest(QStringLiteral("Case2-非法输入-控制点数量不匹配"), poles2,knots2, mults2, 3, false, gp_Vec(0, 0, 0));

    //case3 非法输入：节点与重数不匹配
    std::vector<gp_Pnt> poles3 =
    {
        gp_Pnt(0,   0, 0),
        gp_Pnt(30,  60, 0),
        gp_Pnt(60, -30, 0),
        gp_Pnt(90,  80, 0),
        gp_Pnt(120,  -20, 0),
        gp_Pnt(150,  50, 0),
                gp_Pnt(180,  0, 0)
    };
    std::vector<double> knots3 = { 0,1,2,3,4 };
    std::vector<int> mults3 = { 4, 1,  1, 4 };
    RunBsplineValidityTest(QStringLiteral("Case3-非法输入-节点与重数不匹配"), poles3, knots3, mults3, 3, false, gp_Vec(0, 0, 0));

    //case4 非法输入：节点存在重复
    std::vector<gp_Pnt> poles4 =
    {
        gp_Pnt(0,   0, 0),
        gp_Pnt(30,  60, 0),
        gp_Pnt(60, -30, 0),
        gp_Pnt(90,  80, 0),
        gp_Pnt(120,  -20, 0),
        gp_Pnt(150,  50, 0),
                gp_Pnt(180,  0, 0)
    };
    std::vector<double> knots4 = { 0,1,1,3,4 };
    std::vector<int> mults4 = { 4, 1,  1, 4 };
    RunBsplineValidityTest(QStringLiteral("Case4-非法输入-节点存在重复"), poles4, knots4, mults4, 3, false, gp_Vec(0, 0, 0));

    //case5 非法输入：内部接重数过大
    std::vector<gp_Pnt> poles5 =
    {
        gp_Pnt(0,   0,  0),
        gp_Pnt(35, 75,  0),
        gp_Pnt(70, 15,  0),
        gp_Pnt(105,-15, 0),
        gp_Pnt(140,-75, 0)
    };
    std::vector<double> knots5 = { 0,1,2 };
    std::vector<int> mults5 = { 4, 4, 4 };
    RunBsplineValidityTest(QStringLiteral("Case4-非法输入-内部接重数过大"), poles5, knots5, mults5, 3, false, gp_Vec(0, 0, 0));

}

//  执行单个B样条合法性
void HomePageActionFun::RunBsplineValidityTest(const QString& caseName, const std::vector<gp_Pnt>& poles, const std::vector<double>& knots, const std::vector<int>& mults, int degree, bool needDisplay, const gp_Vec& displayTranslation)
{
    m_outputFunc(QStringLiteral("------------------------------------------------------------"));
    m_outputFunc(QStringLiteral("[%1]").arg(caseName));

    QString checkMsg = BSplineValidationUtils::ValidateOpenBSplineInput(poles, knots, mults, degree);
    m_outputFunc(QStringLiteral("校验结果:%1").arg(checkMsg));

    QString buildMsg;
    Handle(Geom_BSplineCurve) curve = BSplineValidationUtils::CreateOpenBSpline3dChecked(poles, knots, mults, degree, buildMsg);
    m_outputFunc(QStringLiteral("构造结果:%1").arg(buildMsg));

	if (!curve.IsNull() && needDisplay)
    {
        BSplineDisplayStyle style;
        style.translation = displayTranslation;
        style.curveColor = BSplineValidationUtils::MakeColor(0.10, 0.45, 0.90);     //蓝色：曲线
		style.polygonColor = BSplineValidationUtils::MakeColor(0.55, 0.55, 0.55);   //灰色：控制多边形
		style.poleColor = BSplineValidationUtils::MakeColor(0.85, 0.05, 0.05);      //红色：控制点
        BSplineValidationUtils::DisplayCurveCase(m_context, curve, style, false);
        m_outputFunc(BSplineValidationUtils::BuildCurveSummary(caseName, curve));
    }
    m_outputFunc(QStringLiteral("------------------------------------------------------------"));
}