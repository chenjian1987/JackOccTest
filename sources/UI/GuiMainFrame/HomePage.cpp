#include <vector>
#include <tuple>

#include "HomePage.h"
#include "IMainWindow.h"
#include "MainWindow.h"
#include "JNew.h"

#include <QApplication>
#include <QScreen>
#include <QLabel>
#include <QListView>
#include <QResizeEvent>
#include <QHBoxLayout>

#include <cmath>
#include <DebugMessage.h>
#include "GeometryUtilityOccDefine.h"

using namespace JackC;





HomePage::HomePage(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags)
{
    setAttribute(Qt::WA_NativeWindow);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
    setBackgroundRole(QPalette::NoRole);
    setFocusPolicy(Qt::StrongFocus);
    setUpdatesEnabled(false); 

    InitUI();
    m_mainPage = qobject_cast<MainWindow*>(parent);

    m_panSpeed = std::max<qreal>(1.0, devicePixelRatioF());
}

HomePage::~HomePage() {}



void HomePage::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    Initialize();
}

void HomePage::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (!m_v3dView.IsNull())
    {
        m_v3dView->MustBeResized();
        m_v3dView->Redraw();
    }
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

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_pOcctViewContainer);
}

void HomePage::Initialize()
{
    if (!m_context.IsNull()) return; // 仅初始化一次

    // 图形驱动
    m_displayConnection = new Aspect_DisplayConnection();
    m_graphicDriver = new OpenGl_GraphicDriver(m_displayConnection);

    WId winHandle = m_pOcctViewContainer->winId();
    m_occtWindow = new WNT_Window((Aspect_Handle)winHandle);
    if (m_occtWindow.IsNull())
    {
        DBG_WARN(L"Failed to create WNT_Window");
        return;
    }

    // Viewer/View
    m_v3dViewer = new V3d_Viewer(m_graphicDriver);
    m_v3dViewer->SetDefaultLights();
    m_v3dViewer->SetLightOn();

    m_v3dView = m_v3dViewer->CreateView();
    m_v3dView->SetWindow(m_occtWindow);
    if (!m_occtWindow->IsMapped()) { m_occtWindow->Map(); }

    // 交互上下文
    m_context = new AIS_InteractiveContext(m_v3dViewer);

    // 右上角 ViewCube
    Handle(AIS_ViewCube) view_cube = new AIS_ViewCube();
    view_cube->SetBoxColor(Quantity_NOC_GRAY80);
    view_cube->SetTextColor(Quantity_NOC_BLACK);
    view_cube->SetFontHeight(12.0);
    view_cube->SetInnerColor(Quantity_NOC_GRAY60);
    Handle(Graphic3d_TransformPers) transform_pers =
        new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers,
            Aspect_TOTP_RIGHT_UPPER,
            Graphic3d_Vec2i(85, 85));
    view_cube->SetTransformPersistence(transform_pers);
    m_context->Display(view_cube, Standard_False);

    // 显示/选择高亮风格
    m_context->SetDisplayMode(AIS_Shaded, Standard_False);

    {
        Handle(Prs3d_Drawer) hi = m_context->HighlightStyle();
        hi->SetMethod(Aspect_TOHM_COLOR);
        hi->SetColor(Quantity_NOC_LIGHTYELLOW);
        hi->SetDisplayMode(1);
        hi->SetTransparency(0.2f);

        Handle(Prs3d_Drawer) sel = m_context->SelectionStyle();
        sel->SetMethod(Aspect_TOHM_COLOR);
        sel->SetColor(Quantity_NOC_LIGHTSEAGREEN);
        sel->SetDisplayMode(1);
        sel->SetTransparency(0.4f);
    }

    // 背景与初始视角
    m_v3dView->SetBackgroundColor(Quantity_NOC_WHITE);

    InitGrid();
    InitTrihedron();
    InitOriginMarker();

    // 初始适配（避免 SetZoom(10) 这种突兀缩放）
    m_v3dView->FitAll();
    m_v3dView->ZFitAll();
    m_v3dView->Redraw();
    m_v3dView->MustBeResized();

    // 外部动作回调
    opHomePageActionFun = NEW_AS_OWNER_PTR(
        HomePageActionFun, m_context, m_v3dViewer, m_v3dView, this,
        [this](const QString& text) { this->AppendOutput(text); });
}

void HomePage::InitGrid()
{
    // 先确保激活的是矩形网格 + 线条显示
    m_v3dViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
    // 你的老参数：每 100mm/100mm 一条主网格
    m_v3dViewer->SetRectangularGridValues(
        0.0, 0.0,    // 原点 (X0, Y0)
        100.0,       // X 方向步长（模型单位）
        100.0,       // Y 方向步长（模型单位）
        0.0          // 网格旋转角
    );
    m_v3dViewer->SetRectangularGridGraphicValues(
        10000.0,     // X 方向像素间距
        10000.0,     // Y 方向像素间距
        0.0
    );

    // 打开网格显示
    m_v3dView->SetGridActivity(Standard_True);
    // 立刻重绘，确保可见
    m_v3dView->Redraw();
}

void HomePage::InitTrihedron()
{
    gp_Ax2 axis(gp::Origin(), gp::DZ(), gp::DX());
    Handle(Geom_Axis2Placement) placement = new Geom_Axis2Placement(axis);
    Handle(AIS_Trihedron) aisTrihedron = new AIS_Trihedron(placement);

    Handle(Prs3d_DatumAspect) datumAspect = new Prs3d_DatumAspect();
    datumAspect->SetDrawLabels(Standard_True);
    datumAspect->SetDrawArrows(Standard_True);
    datumAspect->SetAxisLength(100.0, 100.0, 100.0);

    datumAspect->LineAspect(Prs3d_DP_XAxis)->SetWidth(3.0);
    datumAspect->LineAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
    datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
    datumAspect->TextAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);

    datumAspect->LineAspect(Prs3d_DP_YAxis)->SetWidth(3.0);
    datumAspect->LineAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datumAspect->TextAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);

    datumAspect->LineAspect(Prs3d_DP_ZAxis)->SetWidth(3.0);
    datumAspect->LineAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);
    datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);
    datumAspect->TextAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);

    Handle(Prs3d_Drawer) drawer = new Prs3d_Drawer();
    drawer->SetDatumAspect(datumAspect);
    aisTrihedron->SetAttributes(drawer);

    Handle(Graphic3d_TransformPers) tp =
        new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers,
            Aspect_TOTP_LEFT_LOWER,
            Graphic3d_Vec2i(85, 85));
    aisTrihedron->SetTransformPersistence(tp);

    m_context->Display(aisTrihedron, Standard_False);
    m_context->Redisplay(aisTrihedron, Standard_False);
}

void HomePage::InitOriginMarker()
{
    Handle(Geom_Point) geomPoint = new Geom_CartesianPoint(gp_Pnt(0.0, 0.0, 0.0));
    Handle(AIS_Point)  aisPoint = new AIS_Point(geomPoint);
    Handle(Prs3d_PointAspect) pointAspect =
        new Prs3d_PointAspect(Aspect_TOM_POINT, Quantity_NOC_RED, 5.0);
    aisPoint->Attributes()->SetPointAspect(pointAspect);
    m_context->Display(aisPoint, Standard_False);
}



void HomePage::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_context.IsNull() || m_v3dView.IsNull()) return;

    m_rotating = false;
    m_panning = false;

    m_context->MoveTo((int)event->position().x(), (int)event->position().y(), m_v3dView, Standard_True);
    m_v3dView->Redraw();
}

void HomePage::mousePressEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton))
    {
        // 左右键齐按：初始化平移
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
    }
    else if (event->button() == Qt::MiddleButton)
    {
        // 中键按下：初始化平移锚点
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
    }
    else if (event->button() == Qt::LeftButton)
    {
        // 左键：选择
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_v3dView, Standard_True);
        if (qApp->keyboardModifiers() == Qt::ControlModifier)
            m_context->SelectDetected(AIS_SelectionScheme_Add);
        else
            m_context->SelectDetected();
        m_v3dView->Redraw();
    }
    else if (event->button() == Qt::RightButton)
    {
        // 右键：只做旋转，不要 Pan
        m_v3dView->StartRotation(event->pos().x(), event->pos().y());
    }
}

void HomePage::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MiddleButton)
    {
        // 中键拖动：平移（用像素差）
        m_v3dView->Pan(event->pos().x() - m_xmax, m_ymax - event->pos().y());
        m_xmax = event->pos().x();
        m_ymax = event->pos().y();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        // 右键拖动：旋转（轨迹球）
        m_v3dView->Rotation(event->pos().x(), event->pos().y());
    }
    else
    {
        // hover 高亮
        m_context->MoveTo(event->pos().x(), event->pos().y(), m_v3dView, Standard_True);
    }
}

void HomePage::wheelEvent(QWheelEvent* event)
{
    if (m_v3dView.IsNull()) return;

    const int x = static_cast<int>(event->position().x());
    const int y = static_cast<int>(event->position().y());
    const int delta = event->angleDelta().y(); // 每格一般 ±120
    if (delta == 0) return;

    const int steps = delta / 120;
    m_v3dView->StartZoomAtPoint(x, y);
    if (steps > 0)
    {
        for (int i = 0; i < steps; ++i)
            m_v3dView->ZoomAtPoint(x, y, x + 1, y + 1);
    }
    else
    {
        for (int i = 0; i < -steps; ++i)
            m_v3dView->ZoomAtPoint(x, y, x - 1, y - 1);
    }

    // 防止缩放比例越界导致“看不到/看起来没反应”
    const Standard_Real minS = 1e-7, maxS = 1e7;
    const Standard_Real s = m_v3dView->Scale();
    if (s < minS) m_v3dView->SetScale(minS);
    if (s > maxS) m_v3dView->SetScale(maxS);

    m_v3dView->Redraw();
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
    if (m_mainPage) m_mainPage->AppendOutput(text);
}
