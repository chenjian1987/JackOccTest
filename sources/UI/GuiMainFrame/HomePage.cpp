#include<vector>
#include<tuple>


#include "HomePage.h"
#include "IMainWindow.h"
#include "MainWindow.h"
#include "JNew.h"

#include <QLabel>
#include <QResizeEvent>
#include <QLayout>
#include <QPushButton>
#include <QApplication>
#include <QScreen>
#include <QMdiArea>
#include <QListView>


#include "GuiMainOccDefines.h"

#include <cmath>
#include <DebugMessage.h>

using namespace JackC;




HomePage::HomePage(QWidget* parent, Qt::WindowFlags flags) : QWidget(parent, flags)
{
    setAttribute(Qt::WA_NativeWindow);

    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setUpdatesEnabled(false); // 防止Qt干扰渲染

    InitUI();
    m_mainPage =(MainWindow*) parent;
}
HomePage::~HomePage() {}



void HomePage::showEvent(QShowEvent* event) 
{
    Initialize();


}
void HomePage::resizeEvent(QResizeEvent* event) 
{
    QWidget::resizeEvent(event);
    if (!m_v3dView.IsNull()) 
    {
        m_v3dView->MustBeResized();
    }
    
}



// 鼠标中键移动， 右键旋转
void HomePage::mousePressEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton))
    {
        // 鼠标左右键齐按：初始化平移
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
    }
    else if (event->buttons() & Qt::LeftButton)
    {
        // 点击前，将鼠标位置传递到交互环境
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_v3dView, Standard_True);
        // 鼠标左键：选择模型
        AIS_StatusOfPick t_pick_status = AIS_SOP_NothingSelected;
        if (qApp->keyboardModifiers() == Qt::ControlModifier)
        {
            t_pick_status = m_context->SelectDetected(AIS_SelectionScheme_Add);   // 多选
        }
        else
        {
            t_pick_status = m_context->SelectDetected();        // 单选
        }
        m_v3dView->Update();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        // 鼠标滚轮键：初始化平移
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
        // 鼠标滚轮键：初始化旋转
        m_v3dView->StartRotation(event->pos().x(), event->pos().y());
    }
}
//!覆写鼠标按键释放事件
void HomePage::mouseReleaseEvent(QMouseEvent* event)
{
    m_context->MoveTo(event->pos().x(), event->pos().y(), m_v3dView, Standard_True);
}
//!覆写鼠标移动事件
void HomePage::mouseMoveEvent(QMouseEvent* event)
{
    if  (event->buttons() & Qt::RightButton)
    {
        // 鼠标左右键齐按：执行平移
        m_v3dView->Pan(event->pos().x() - m_xmax, m_ymax - event->pos().y());
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
    }
    else if (event->buttons() & Qt::MiddleButton)
    {
        // 鼠标滚轮键
        if (qApp->keyboardModifiers() == Qt::ShiftModifier)    // 且按下Shift键
        {
            // 鼠标滚轮键：执行平移
            m_v3dView->Pan(event->pos().x() - m_xmax, m_ymax - event->pos().y());
            m_xmax = event->pos().x();
            m_ymax = event->pos().y();
        }
        else
        {
            // 鼠标滚轮键：执行旋转
            m_v3dView->Rotation(event->pos().x(), event->pos().y());
        }
    }
    else
    {
        // 将鼠标位置传递到交互环境
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_v3dView, Standard_True);
    }
}
//!覆写鼠标滚轮事件
void HomePage::wheelEvent(QWheelEvent* event)
{
    m_v3dView->StartZoomAtPoint(event->position().x(), event->position().y());
    m_v3dView->ZoomAtPoint(0, 0, event->angleDelta().y(), 0); //执行缩放
}




void HomePage::InitUI()
{
    m_pOcctViewContainer = new QWidget(this);
    m_pOcctViewContainer->setAutoFillBackground(false);
    m_pOcctViewContainer->setAttribute(Qt::WA_NoSystemBackground);
    m_pOcctViewContainer->setAttribute(Qt::WA_PaintOnScreen);
    m_pOcctViewContainer->setBackgroundRole(QPalette::NoRole);
    m_pOcctViewContainer->setFocusPolicy(Qt::StrongFocus);
    m_pOcctViewContainer->setUpdatesEnabled(false);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_pOcctViewContainer);

}

void HomePage::Initialize() 
{
    // 仅初始化一次
    if (m_context.IsNull()) 
    {
        // 初始化图形驱动
        m_displayConnection = new Aspect_DisplayConnection();
        m_graphicDriver = new OpenGl_GraphicDriver(m_displayConnection);

        WId winHandle = m_pOcctViewContainer->winId();

        m_occtWindow = new WNT_Window((Aspect_Handle)winHandle);
        if (m_occtWindow.IsNull()) 
        {
            DBG_WARN(L"Failed to create WNT_Window");
            return;
        }
        m_v3dViewer = new V3d_Viewer(m_graphicDriver);
        m_v3dViewer->SetDefaultLights();
        m_v3dViewer->SetLightOn();
        m_v3dView = m_v3dViewer->CreateView();
        m_v3dView->SetWindow(m_occtWindow);
        if (!m_occtWindow->IsMapped()) {
            m_occtWindow->Map();
        }
        m_context = new AIS_InteractiveContext(m_v3dViewer);
        //添加右上角cube框    
        auto view_cube = new AIS_ViewCube();    
        auto transform_pers = new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers, Aspect_TOTP_RIGHT_UPPER, Graphic3d_Vec2i(85, 85));    
        view_cube->SetTransformPersistence(transform_pers);    
        m_context->Display(view_cube, Standard_True);

        //设置显示模式    
        m_context->SetDisplayMode(AIS_Shaded, Standard_True);
        // 设置模型高亮的风格    
        Handle(Prs3d_Drawer) t_hilight_style = m_context->HighlightStyle(); 
        // 获取高亮风格    
        t_hilight_style->SetMethod(Aspect_TOHM_COLOR);  
        // 颜色显示方式   
        t_hilight_style->SetColor(Quantity_NOC_LIGHTYELLOW);    
        // 设置高亮颜色    
        t_hilight_style->SetDisplayMode(1); 
        // 整体高亮    
        t_hilight_style->SetTransparency(0.2f); 

        //设置选择模型的风格    
        Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();  
        // 获取选择风格    
        t_select_style->SetMethod(Aspect_TOHM_COLOR);  
        // 颜色显示方式    
        t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   
        // 设置选择后颜色    
        t_select_style->SetDisplayMode(1); 
        // 整体高亮    
        t_select_style->SetTransparency(0.4f); // 设置透明度 

        m_v3dView->SetZoom(10);

        InitGrid();
        InitTrihedron();

        // 视图适配
        m_v3dView->SetBackgroundColor(Quantity_NOC_WHITE);
        m_v3dView->FitAll();
        m_v3dView->Redraw();
        m_v3dView->MustBeResized();
    }
    opHomePageActionFun = NEW_AS_OWNER_PTR(HomePageActionFun,m_context, m_v3dViewer, m_v3dView,this, [this](const QString& text) { this->AppendOutput(text); });
    InitOriginMarker();
}
void HomePage::InitGrid()
{
    m_v3dViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
    // 设置网格参数：逻辑原点和步长（模型单位）
    m_v3dViewer->SetRectangularGridValues(0.0, 0.0, 10.0, 10.0, 0.0);
    // 设置网格在屏幕上的显示比例（像素单位）
    m_v3dViewer->SetRectangularGridGraphicValues(100.0, 100.0, 0.0);
    // 启用视图内的网格显示
    m_v3dView->SetGridActivity(Standard_True);
}
void HomePage::InitTrihedron()
{
    gp_Ax2 axis(gp::Origin(), gp::DZ(), gp::DX());
   
    // 创建坐标系
    Handle(Geom_Axis2Placement) placement = new Geom_Axis2Placement(axis);
    Handle(AIS_Trihedron) aisTrihedron = new AIS_Trihedron(placement);

    // 设置自定义Drawer和DatumAspect
    Handle(Prs3d_Drawer) drawer = new Prs3d_Drawer();
    Handle(Prs3d_DatumAspect) datumAspect = new Prs3d_DatumAspect();

    // 明确启用彩色显示模式（最关键的一步！）
    datumAspect->SetDrawLabels(Standard_True);          // 显示标签
    datumAspect->SetDrawArrows(Standard_True);          // 显示箭头
    datumAspect->SetAxisLength(100.0, 100.0, 100.0);    // 根据实际需求设置轴长

    // 设置轴颜色
    datumAspect->LineAspect(Prs3d_DP_XAxis)->SetWidth(5.0);                     // 设置轴线宽度
    datumAspect->LineAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);        //控制轴杆外边缘线的颜色和线宽
    datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);     //控制轴杆本体颜色
    datumAspect->TextAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);        //控制标签文字颜色

    datumAspect->LineAspect(Prs3d_DP_YAxis)->SetWidth(5.0);         // 设置轴线宽度
    datumAspect->LineAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datumAspect->TextAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);

    datumAspect->LineAspect(Prs3d_DP_ZAxis)->SetWidth(5.0);         // 设置轴线宽度
    datumAspect->LineAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE);
    datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE);
    datumAspect->TextAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE);


    // 应用DatumAspect到Drawer
    drawer->SetDatumAspect(datumAspect);
    aisTrihedron->SetAttributes(drawer);

    Handle(Graphic3d_TransformPers) transformPers = new Graphic3d_TransformPers(
        Graphic3d_TMF_TriedronPers,
        Aspect_TOTP_LEFT_LOWER,
        Graphic3d_Vec2i(85, 85)
    );
    aisTrihedron->SetTransformPersistence(transformPers);

    // 强制更新显示
    m_context->Display(aisTrihedron, Standard_True);
    m_context->Redisplay(aisTrihedron, Standard_True);


}

void HomePage::ShowMaximized()
{
    QWidget::showMaximized();
}
void HomePage::ShowMinimized()
{
    QWidget::showMinimized();
}

void HomePage::AppendOutput(const QString& text)
{
    m_mainPage->AppendOutput(text);
}


void HomePage::InitOriginMarker()
{
    // 创建一个 Geom_Point
    Handle(Geom_Point) geomPoint = new Geom_CartesianPoint(gp_Pnt(0.0, 0.0, 0.0));

    // 用 Geom_Point 创建 AIS_Point
    Handle(AIS_Point) aisPoint = new AIS_Point(geomPoint);

    // 设置显示样式
    Handle(Prs3d_PointAspect) pointAspect = new Prs3d_PointAspect(
        Aspect_TOM_POINT,
        Quantity_NOC_RED,
        5.0 // 点大小
    );
    aisPoint->Attributes()->SetPointAspect(pointAspect);

    // 显示
    m_context->Display(aisPoint, Standard_True);
}