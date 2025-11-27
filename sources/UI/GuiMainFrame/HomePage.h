#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "GuiMainFrameDefine.h"
#include "IMainWindow.h"
#include <QWidget>
#include <QRubberBand>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QShowEvent>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QPoint>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Point.hxx>
#include <AIS_ViewCube.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <gp_Ax2.hxx>

#include "HomePageActionFun.h"

class QPushButton;
class QLabel;
class QHBoxLayout;

namespace JackC
{
    class MainWindow;

    class JACKC_GUI_MAIN_FRAME_EXPORT HomePage : public QWidget
    {
        Q_OBJECT
    public:
        explicit HomePage(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags(0));
        ~HomePage() override;

        void ShowMaximized() ;
        void ShowMinimized() ;

        void AppendOutput(const QString& text);

    protected:
        void showEvent(QShowEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        void paintEvent(QPaintEvent*) override {} // 禁止Qt绘制，交给OCCT
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;

    private:
        void InitUI();
        void Initialize();
        void InitTrihedron();
        void InitGrid();
        void InitOriginMarker();

    private:
        MainWindow* m_mainPage = nullptr;

        QWidget* m_pOcctViewContainer = nullptr;
        Handle(Aspect_DisplayConnection) m_displayConnection;
        Handle(OpenGl_GraphicDriver)     m_graphicDriver;
        Handle(V3d_Viewer)               m_v3dViewer;
        Handle(V3d_View)                 m_v3dView;
        Handle(AIS_InteractiveContext)   m_context;
        Handle(WNT_Window)               m_occtWindow;

        QWidget* m_pContentView = nullptr;
        QHBoxLayout* m_pHBGContentLayout = nullptr;
        QLabel* m_pWelcomeTitleLabel = nullptr;
        QWidget* m_pLeftContentView = nullptr;
        QPushButton* m_pNewFileButton = nullptr;
        QPushButton* m_pOpenFileButton = nullptr;
        QPushButton* m_pAboutButton = nullptr;

        QPoint m_lastPos;          // 上一帧鼠标位置（像素）
        bool   m_rotating = false;
        bool   m_panning = false;
        double m_panSpeed = 1.0;   // DPI 自适应平移速度

        Standard_Integer m_xmin = 0;
        Standard_Integer m_ymin = 0;
        Standard_Integer m_xmax = 0;
        Standard_Integer m_ymax = 0;

        QRubberBand* m_currentRectBand = nullptr;

    public:
        OwnerPtr<HomePageActionFun> opHomePageActionFun;
    };
} // namespace JackC

#endif // !MAINVIEW_H
