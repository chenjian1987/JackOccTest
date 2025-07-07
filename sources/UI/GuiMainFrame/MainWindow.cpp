#include <QApplication>
#include <QMdiArea>
#include <QPoint>
#include <QToolBar>
#include <QWidget>
#include <QObject>
#include <QKeySequence>
#include <QLayout>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QDebug>
#include <QDir>
#include <QFile>

#include <QScreen>
#include <SAFramelessHelper.h>


#include "MainWindow.h"
#include "UiManager.h"
#include "DebugMessage.h"
#include "JNew.h"
#include "FilePathManager.h"
#include "HomePage.h"


using namespace JackC;




namespace
{
    class JackCMdiArea : public QMdiArea
    {
    public:
        JackCMdiArea(QWidget* pParent) : QMdiArea(pParent)
        {
        }

    private:
        void wheelEvent(QWheelEvent* pEvent) override
        {
            (pEvent);
        }
    };
}




MainWindow::MainWindow(QWidget* parent) :SARibbonMainWindow(parent), m_isAppClosing(false)
{
    UiManager* pUiMgr = UiManager::GetInstance();
    DBG_WARN_UNLESS(pUiMgr, L"pUiMgr is nullptr", L"jack.chen", L"2025-05-10");
    pUiMgr->SetApplicationWindow(this);

    // 设置默认字体（如DockWidget的Tab控件）
    QString fontFamily = QString::fromStdWString(L"Microsoft YaHei");
    QFont   font(fontFamily);
    setFont(font);
}
MainWindow::~MainWindow()
{
}



bool MainWindow::Initialize()
{
    // 设置窗口icon
    FilePathManager* pFilePathManager = FilePathManager::Get();
    DBG_WARN_AND_RETURN_FALSE_UNLESS(pFilePathManager, L"pFilePathManager为空", L"jack.chen", L"2025-05-10");
    const FilePath& resourcePath = pFilePathManager->GetResourcePath();
    std::wstring    strIconFilePath(resourcePath.GetFullPath() + L"/JackCAD.png");
    setWindowIcon(QIcon(QString::fromStdWString(strIconFilePath)));

    // 设置窗口标题
    setWindowTitle(QString::fromLocal8Bit("JackQuickOCCT"));
    setMinimumSize(QSize(1288, 1000));
    showNormal();

    //添加工具栏
    QToolBar* toolBar = new QToolBar();
    toolBar->setMovable(false);         //设置工具栏不可移动
    addToolBar(toolBar);

    createMenus();
    createToolBars();
    createActions();    

    HomePage* pHomePage = NEW_AS_QT(HomePage, this);
    m_pHomePage = pHomePage;

    // 创建容器
    QWidget* central = new QWidget(this);
    QVBoxLayout* pVLayout = new QVBoxLayout(central);
    pVLayout->setContentsMargins(0, 0, 0, 0);
    pVLayout->setSpacing(0);

    // 主视图
    pVLayout->addWidget(pHomePage, 1);

    // 日志
    m_outputWindow = new QPlainTextEdit(central);
    m_outputWindow->setReadOnly(true);
    m_outputWindow->setFixedHeight(150);
    m_outputWindow->setStyleSheet("background-color: white; color: black; font-family: Consolas;");

    pVLayout->addWidget(m_outputWindow, 0);

    setCentralWidget(central);
    return true;
}



void MainWindow::createMenus()
{
    SARibbonBar* menu_TestBrep = ribbonBar();
    menu_TestBrep->setRibbonStyle(SARibbonBar::RibbonStyle::RibbonStyleCompactThreeRow);
    menu_TestBrep->setFont(QFont("微软雅黑", 10));
    menu_TestBrep->applicationButton()->setVisible(false);
}


void MainWindow::createToolBars()
{
    SARibbonBar* mainPageRibbon = ribbonBar();
    SARibbonCategory* categoryPanelTestBRep= new SARibbonCategory();
    categoryPanelTestBRep->setCategoryName(QString::fromLocal8Bit("测试BRep"));
    categoryPanelTestBRep->setObjectName("测试BRep");
    categoryPanelTestBRep->setFont(QFont(QString::fromLocal8Bit("微软雅黑"), 10));
    mainPageRibbon->addCategoryPage(categoryPanelTestBRep);

    // geom curve
    m_panelCurve = categoryPanelTestBRep->addPannel(QString::fromLocal8Bit("测试Geom Curve"));
    //brep solid
    m_panelSolid = categoryPanelTestBRep->addPannel(QString::fromLocal8Bit("测试BRep Solid"));
    m_panelEdit = categoryPanelTestBRep->addPannel(QString::fromLocal8Bit("    互操作 & 互编辑   "));

    //视图
    SARibbonBar* barView=ribbonBar();
    SARibbonCategory* categoryView=new SARibbonCategory();
    categoryView->setCategoryName(QString::fromLocal8Bit("视图"));
    categoryView->setObjectName("视图");
    categoryView->setFont(QFont(QString::fromLocal8Bit("微软雅黑"),10));
    barView->addCategoryPage(categoryView);

    m_coreViewControl=categoryView->addPannel(QString::fromLocal8Bit("视图"));

    // 数据中心
    SARibbonBar* dataCenterBarView = ribbonBar();
    SARibbonCategory* dataCenterView = new SARibbonCategory();
    dataCenterView->setCategoryName(QString::fromLocal8Bit("数据中心"));
    dataCenterView->setObjectName("数据中心");
    dataCenterView->setFont(QFont(QString::fromLocal8Bit("微软雅黑"), 10));
    dataCenterBarView->addCategoryPage(dataCenterView);

    m_ifcDataManager = dataCenterView->addPannel(QString::fromLocal8Bit("IFC"));
}



void MainWindow::createActions() 
{
    QString exePath =QApplication::applicationDirPath();
    // geom curve
    QString splitCurvePath= exePath +QString("/Resource/Arc3Points.png");
    m_actionSimpleBSplineCurve = new QAction(QIcon(splitCurvePath), QString::fromLocal8Bit("Simple BSplineCurve"), m_panelCurve);
    connect(m_actionSimpleBSplineCurve, &QAction::triggered, this, &MainWindow::onActionCreateSimpleBSplineCurve);
    m_panelCurve->addSmallAction(m_actionSimpleBSplineCurve);

    //brep solid
    QString simpleSolidPath = exePath + QString("/Resource/DisplayMode_Shaded.png");
    m_actionSimpleSolidTranslate = new QAction(QIcon(simpleSolidPath), QString::fromLocal8Bit("Simple BEep Build And Translate"), m_panelSolid);
    connect(m_actionSimpleSolidTranslate, &QAction::triggered, this, &MainWindow::onActionSimpleBuildAndTranslate);
    m_panelSolid->addSmallAction(m_actionSimpleSolidTranslate);

    m_actionTopExp = new QAction(QIcon(simpleSolidPath), QString::fromLocal8Bit("Simple Top Exp"), m_panelSolid);
    connect(m_actionTopExp, &QAction::triggered, this, &MainWindow::onActionSimpleTopExp);
    m_panelSolid->addSmallAction(m_actionTopExp);

    m_actionInclineRectangle = new QAction(QIcon(simpleSolidPath), QString::fromLocal8Bit("创建倾斜的长方体"), m_panelSolid);
    connect(m_actionInclineRectangle, &QAction::triggered, this, &MainWindow::onActionCreateRectangle);
    m_panelSolid->addSmallAction(m_actionInclineRectangle);

    //edit 
    QString rotationSolidPath = exePath + QString("/Resource/ExternalCommands.png");
    m_actionEditShape = new QAction(QIcon(rotationSolidPath), QString::fromLocal8Bit("互操作"), m_panelEdit);
    connect(m_actionEditShape, &QAction::triggered, this, &MainWindow::onActionEditShape);
    m_panelEdit->addSmallAction(m_actionEditShape);

    //QString rotationSolidPath = exePath + QString("/Resource/ExternalCommands.png");
    m_actionBuildBaseMode = new QAction(QIcon(rotationSolidPath), QString::fromLocal8Bit("基础建模"), m_panelEdit);
    connect(m_actionBuildBaseMode, &QAction::triggered, this, &MainWindow::onActionBuildBaseMode);
    m_panelEdit->addSmallAction(m_actionBuildBaseMode);


    // view
    m_actionLeftCoreView = new QAction(QIcon(QString(exePath + "/Resource/Floor.png")), QString::fromLocal8Bit("左视图"), m_panelSolid);
    connect(m_actionLeftCoreView, &QAction::triggered, this, &MainWindow::onActionLeftCoreView);
    m_coreViewControl->addSmallAction(m_actionLeftCoreView);

    m_actionRightCoreView = new QAction(QIcon(QString(exePath + "/Resource/Floor.png")), QString::fromLocal8Bit("右视图"), m_panelSolid);
    connect(m_actionRightCoreView, &QAction::triggered, this, &MainWindow::onActionRightCoreView);
    m_coreViewControl->addSmallAction(m_actionRightCoreView);

    m_actionFrontCoreView = new QAction(QIcon(QString(exePath + "/Resource/Floor.png")), QString::fromLocal8Bit("前视图"), m_panelSolid);
    connect(m_actionFrontCoreView, &QAction::triggered, this, &MainWindow::onActionFrontCoreView);
    m_coreViewControl->addSmallAction(m_actionFrontCoreView);

    m_actionBackCoreView = new QAction(QIcon(QString(exePath + "/Resource/Floor.png")), QString::fromLocal8Bit("后视图"), m_panelSolid);
    connect(m_actionBackCoreView, &QAction::triggered, this, &MainWindow::onActionBackCoreView);
    m_coreViewControl->addSmallAction(m_actionBackCoreView);

    //数据中心
    m_actionImportIfc = new QAction(QIcon(QString(exePath + "/Resource/Door.png")), QString::fromLocal8Bit("导入IFC"), m_panelSolid);
    connect(m_actionImportIfc, &QAction::triggered, this, &MainWindow::onActionImportIfc);
    m_ifcDataManager->addSmallAction(m_actionImportIfc);
}



void MainWindow::AppendOutput(const QString& text)
{
    if (m_outputWindow)
    {
        m_outputWindow->appendPlainText(text);
    }
}


MAINWINDOW_ACTION_SLOTS_FUN(MainWindow, m_pHomePage, opHomePageActionFun)