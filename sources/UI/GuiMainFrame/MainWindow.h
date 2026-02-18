#pragma once
#include <QString>
#include <SARibbonMainWindow.h>
#include <QMainWindow>
#include "SARibbonContextCategory.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonBar.h"
#include "SARibbonMainWindow.h"

#include "IMainWindow.h"
#include "GuiMainFrameDefine.h"
#include "WinMainSlotsDefine.h"
#include "QPlainTextEdit.h"



class QMdiArea;
class QMdiSubWindow;
class QTimer;
class QObject;


namespace JackC
{
    class HomePage;


    class JACKC_GUI_MAIN_FRAME_EXPORT MainWindow:public SARibbonMainWindow,public IMainWindow
    {
        Q_OBJECT
    public:
        MainWindow(QWidget* parent = Q_NULLPTR);
        virtual ~MainWindow();

    public:
        virtual bool Initialize() override;// 初始化
        void AppendOutput(const QString& text);

    private slots:
        void onActionSimpleBuildAndTranslate(); 
        void onActionSimpleTopExp(); 
        void onActionCreateSimpleBSplineCurve(); 
        void onActionCreateRectangle(); 
        void onActionTestSimpleSewing();   //测试简单缝合
        void onActionTestSimpleShapeUpgrade();   //测试简单简化

        void onActionLeftCoreView(); 
        void onActionRightCoreView(); 
        void onActionFrontCoreView(); 
        void onActionBackCoreView();
        void onActionTopCoreView();
        void onActionBottomCoreView();

        void onActionEditShape();
        void onActionBuildBaseMode();
        void onActionImportIfc();

        void onActionPointLineRelation();
        void onActionPointOnFace();

        void onActionLine2Line();
        void onActionLine2BSplineCurve();
        void onActionSegPlaneIntersection();
        void onActionArcBSplineIntersect();

        void onActionArcFaceIntersectBSplineFace();
        void onActionMatrixTranslate();

        void onActionHelloOCAF();

    private:
        void createActions();
        void createMenus();
        void createToolBars();


    private:
        QPlainTextEdit* m_outputWindow = nullptr;

        // MDI区域。
        QMdiArea* m_pMDIArea;
        HomePage* m_pHomePage;

        // 程序是否正在退出的标志
        bool    m_isAppClosing;
        QToolBar* m_brepToolBar;

        // brep
        SARibbonBar* menu_TestBrep;
        SARibbonPannel* m_panelCurve;
        SARibbonPannel* m_panelSolid;
        SARibbonPannel* m_panelEdit;
        SARibbonPannel* m_panelRelationship;
        SARibbonPannel* m_panelMatrixTranslate;     //坐标系转换

        QAction* m_actionSimpleBSplineCurve;
        QAction*  m_actionSimpleSolidTranslate;
        QAction* m_actionTopExp;
        QAction* m_actionInclineRectangle;
        QAction* m_actionSewingSimpleTest;  //测试简单的缝合
        QAction* m_actionShapeUpgradeSimpleTest;  //测试简单的缝合

        QAction* m_actionEditShape;
        QAction* m_actionBuildBaseMode;
        QAction* m_actionPointLineRelation;
        QAction* m_actionPointOnFace;
        QAction* m_actionLine2Line;
        QAction* m_actionLine2BSplineCurve;
        QAction* m_actionSegmentPlaneIntersection;
        QAction* m_actionArcBsplineInteersect;
        QAction* m_actionArcFaceIntersectBSplineFace;
        QAction* m_actionMatrixTranslate;
        
        //view
        SARibbonPannel* m_coreViewControl;
        QAction* m_actionLeftCoreView;
        QAction* m_actionRightCoreView;
        QAction* m_actionFrontCoreView;
        QAction* m_actionBackCoreView;
        QAction* m_actionTopCoreView;
        QAction* m_actionBottomCoreView;

        // DataCenter
        SARibbonPannel* m_ifcDataManager;
        QAction* m_actionImportIfc;

        SARibbonPannel* m_ocafManager;
        QAction* m_actionHelloOCAF;
    };
}