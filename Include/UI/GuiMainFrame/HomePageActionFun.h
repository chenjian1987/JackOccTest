#pragma once

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <gp_Pnt.hxx>

#include "ShapeEditDialog.h"
#include <GuiMainFrameDefine.h>


class QWidget;

class HomePageActionFun
{
public:
    HomePageActionFun(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView,QWidget* parent );

public:
    void TestSimpleBuildAndTranslate(); 
    void TestSimpleTopExp(); 
    void TestEditShape(); 
    void TestBuildBaseMode();
    
    void TestLeftCoreView(); 
    void TestRightCoreView(); 
    void TestFrontCoreView(); 
    void TestBackCoreView(); 
    
    void TestCreateSimpleBSplineCurve(); 
    void TestCreateRectangle();

    void ImportIfc();
    void ImportIfc(const QString str);

private:
    Handle(AIS_Shape) GetFirstSelectedShape() const;

    void SetStandardView(const Handle(V3d_View)& view, EnumCoreViewControlType v);

    void ApplyTransformToSelected(ShapeEditDialog::TransformType type, QVector3D vec, double value, int mirrorPlane);


private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;

    QWidget* m_parent;
};