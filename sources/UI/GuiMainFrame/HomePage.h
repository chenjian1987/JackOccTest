#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "GuiMainFrameDefine.h"
#include "IMainWindow.h"
#include <QWidget>
#include <QRubberBand>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <gp_Pnt.hxx>

#include "HomePageActionFun.h"

class QPushButton;
class QLabel;
class QHBoxLayout;

namespace JackC 
{



    class JACKC_GUI_MAIN_FRAME_EXPORT HomePage : public QWidget 
    {
        Q_OBJECT

    public:
        HomePage(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags(0));
        virtual ~HomePage();

    public:
        virtual void ShowMaximized();
        virtual void ShowMinimized();

        void AppendOutput(const QString& text);

    protected:
        virtual void showEvent(QShowEvent *event) override;
        virtual void resizeEvent(QResizeEvent *event) override;

    private:
        void InitUI();
        void Initialize();
        void InitTrihedron();
        void InitGrid();

        void mousePressEvent(QMouseEvent* event);
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
        void wheelEvent(QWheelEvent* event);

        void InitOriginMarker();

    private:
        MainWindow* m_mainPage;

        QWidget *m_pOcctViewContainer; 
        Handle(Aspect_DisplayConnection) m_displayConnection;
        Handle(OpenGl_GraphicDriver) m_graphicDriver;
        Handle(V3d_Viewer) m_v3dViewer;
        Handle(V3d_View) m_v3dView;
        Handle(AIS_InteractiveContext) m_context;
        Handle(WNT_Window) m_occtWindow;

        // UI¿Ø¼þ
        QWidget *m_pContentView;
        QHBoxLayout *m_pHBGContentLayout;
        QLabel *m_pWelcomeTitleLabel;
        QWidget *m_pLeftContentView;
        QPushButton *m_pNewFileButton;
        QPushButton *m_pOpenFileButton;
        QPushButton *m_pAboutButton;

        Standard_Integer m_xmin;
        Standard_Integer m_ymin;
        Standard_Integer m_xmax;
        Standard_Integer m_ymax;

        QRubberBand* m_currentRectBand;

    public:
        OwnerPtr<HomePageActionFun> opHomePageActionFun;
    };
} // namespace JackC

#endif // !MAINVIEW_H
