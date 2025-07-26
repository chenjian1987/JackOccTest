#include "WinPointOnSurface.h"
#include <QMessageBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QString>
#include <sstream>
#include <QLabel>
#include <QTabWidget>

#include "GuiMainOccDefines.h"
#include "JNew.h"




WinPointOnSurface::WinPointOnSurface(Handle(AIS_InteractiveContext) context,Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView, QWidget* parent,std::function<void(const QString&)> func)
    : QDialog(parent), m_context(context), m_v3dView(v3dView),   m_v3dViewer(v3dViewer), m_outputFunc(func)
{
    setupUI();

    op_pointPlaneRelationshipUtil = NEW_AS_OWNER_PTR(GeomPointPlanRelationshipUtility);
    op_pointSurfaceRelationUtil = NEW_AS_OWNER_PTR(GeomPointSurfaceRelationshipUtility);
}

void WinPointOnSurface::setupUI()
{
    setWindowTitle(QString::fromLocal8Bit("点是否在面上"));
    setMinimumSize(600, 400);

    QTabWidget* tabWidget = new QTabWidget(this);

    // 创建四个标签页
    m_tabPlane = new QWidget;
    m_tabSphere = new QWidget;
    m_tabCylinder = new QWidget;
    m_tabBSpline = new QWidget;

    // 设置每个标签页的UI
    setupPlaneTab(m_tabPlane);
    setupSphereTab(m_tabSphere);
    setupCylinderTab(m_tabCylinder);
    setupBSplineTab(m_tabBSpline);

    // 添加标签页
    tabWidget->addTab(m_tabPlane, QString::fromLocal8Bit("平面"));
    tabWidget->addTab(m_tabSphere, QString::fromLocal8Bit("球面"));
    tabWidget->addTab(m_tabCylinder, QString::fromLocal8Bit("圆柱面"));
    tabWidget->addTab(m_tabBSpline, QString::fromLocal8Bit("B样条曲面"));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

// 创建点坐标输入布局
QWidget* WinPointOnSurface::createPointInputLayout(QLineEdit*& x, QLineEdit*& y, QLineEdit*& z)
{
    QWidget* widget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(widget);
    x = new QLineEdit("0");
    y = new QLineEdit("0");
    z = new QLineEdit("0");
    layout->addWidget(new QLabel("X:"));
    layout->addWidget(x);
    layout->addWidget(new QLabel("Y:"));
    layout->addWidget(y);
    layout->addWidget(new QLabel("Z:"));
    layout->addWidget(z);
    layout->setContentsMargins(0, 0, 0, 0);
    return widget;
}

// 设置平面标签页
void WinPointOnSurface::setupPlaneTab(QWidget* tab)
{
    // 点输入框
    QWidget* pointInput = createPointInputLayout(m_pointX_plane, m_pointY_plane, m_pointZ_plane);

    // 平面参数输入框
    QWidget* planeParams = new QWidget;
    QHBoxLayout* planeLayout = new QHBoxLayout(planeParams);
    m_planeA = new QLineEdit("0");
    m_planeB = new QLineEdit("0");
    m_planeC = new QLineEdit("1");
    m_planeD = new QLineEdit("0");
    planeLayout->addWidget(new QLabel("A:"));
    planeLayout->addWidget(m_planeA);
    planeLayout->addWidget(new QLabel("B:"));
    planeLayout->addWidget(m_planeB);
    planeLayout->addWidget(new QLabel("C:"));
    planeLayout->addWidget(m_planeC);
    planeLayout->addWidget(new QLabel("D:"));
    planeLayout->addWidget(m_planeD);
    planeLayout->setContentsMargins(0, 0, 0, 0);

    // 计算方法选择
    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethodPlane = new QComboBox;
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("gp_Pln 距离接口"), METHOD_DistanceInterface);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("数学方程计算"), METHOD_Math);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("投影器 (GeomAPI_ProjectPointOnSurf)"), METHOD_Projector);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("分析工具 (ShapeAnalysis_Surface)"), METHOD_AnalysisTool);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("极值计算 (Extrema_ExtPS)"), METHOD_Extrema);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("GeomAdaptor (GeomLProp_SLProps)"), METHOD_GeomAdaptor);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("面域分类器 (BRepClass_FaceClassifier)"), METHOD_FaceClassifier);
    m_comboMethodPlane->addItem(QString::fromLocal8Bit("距离计算 (BRepExtrema_DistShapeShape)"), METHOD_DistShapeShape);

    // 计算按钮
    m_btnComputePlane = new QPushButton(QString::fromLocal8Bit("计算"));
    connect(m_btnComputePlane, &QPushButton::clicked, this, &WinPointOnSurface::onComputePlane);

    // 清空按钮
    m_btnClearPlane = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnClearPlane, &QPushButton::clicked, this, &WinPointOnSurface::onClear);

    // 结果标签
    m_labelResultPlane = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("点坐标:"), pointInput);
    formLayout->addRow(QString::fromLocal8Bit("平面参数 A, B, C, D:"), planeParams);
    formLayout->addRow(methodLabel, m_comboMethodPlane);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputePlane);
    btnLayout->addWidget(m_btnClearPlane);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultPlane);
}

// 设置球面标签页
void WinPointOnSurface::setupSphereTab(QWidget* tab)
{
    // 点输入框
    QWidget* pointInput = createPointInputLayout(m_pointX_sphere, m_pointY_sphere, m_pointZ_sphere);

    // 球心坐标输入框
    QWidget* centerInput = createPointInputLayout(m_sphereCenterX, m_sphereCenterY, m_sphereCenterZ);

    // 半径输入框
    QWidget* radiusInput = new QWidget;
    QHBoxLayout* radiusLayout = new QHBoxLayout(radiusInput);
    m_sphereRadius = new QLineEdit("10");
    radiusLayout->addWidget(new QLabel(QString::fromLocal8Bit("半径:")));
    radiusLayout->addWidget(m_sphereRadius);

    // 计算方法选择
    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethodSphere = new QComboBox;
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("距离球心法"), 0);
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("投影法 GeomAPI_ProjectPointOnSurf"), 1);
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("极值计算 Extrema_ExtPS"), 2);
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("面分析计算 ShapeAnalysis_Surface"), 3);
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("面域分类器 BRepClass_FaceClassifier"), 4);
    m_comboMethodSphere->addItem(QString::fromLocal8Bit("Topo距离计算 BRepExtrema_DistShapeShape"), 5);

    // 计算按钮
    m_btnComputeSphere = new QPushButton(QString::fromLocal8Bit("计算"));
    connect(m_btnComputeSphere, &QPushButton::clicked, this, &WinPointOnSurface::onComputeSphere);

    // 清空按钮
    m_btnClearSphere = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnClearSphere, &QPushButton::clicked, this, &WinPointOnSurface::onClear);

    // 结果标签
    m_labelResultSphere = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("点坐标:"), pointInput);
    formLayout->addRow(QString::fromLocal8Bit("球心坐标:"), centerInput);
    formLayout->addRow(QString::fromLocal8Bit("球面参数:"), radiusInput);
    formLayout->addRow(methodLabel, m_comboMethodSphere);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeSphere);
    btnLayout->addWidget(m_btnClearSphere);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultSphere);
}

// 设置圆柱面标签页
void WinPointOnSurface::setupCylinderTab(QWidget* tab)
{
    // 点输入框
    QWidget* pointInput = createPointInputLayout(m_pointX_cylinder, m_pointY_cylinder, m_pointZ_cylinder);

    // 轴心坐标输入框
    QWidget* axisInput = createPointInputLayout(m_cylinderAxisX, m_cylinderAxisY, m_cylinderAxisZ);

    // 半径和高度输入框
    QWidget* paramsInput = new QWidget;
    QHBoxLayout* paramsLayout = new QHBoxLayout(paramsInput);
    m_cylinderRadius = new QLineEdit("10");
    m_cylinderHeight = new QLineEdit("50");
    paramsLayout->addWidget(new QLabel(QString::fromLocal8Bit("半径:")));
    paramsLayout->addWidget(m_cylinderRadius);
    paramsLayout->addWidget(new QLabel(QString::fromLocal8Bit("高度:")));
    paramsLayout->addWidget(m_cylinderHeight);

    // 计算方法选择
    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethodCylinder = new QComboBox;
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("距离中心线法"), 0);
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("投影法 GeomAPI_ProjectPointOnSurf"), 1);
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("极值计算 Extrema_ExtPS"), 2);
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("面分析计算 ShapeAnalysis_Surface"), 3);
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("面域分类器 BRepClass_FaceClassifier"), 4);
    m_comboMethodCylinder->addItem(QString::fromLocal8Bit("Topo距离计算 BRepExtrema_DistShapeShape"), 5);

    // 计算按钮
    m_btnComputeCylinder = new QPushButton(QString::fromLocal8Bit("计算"));
    connect(m_btnComputeCylinder, &QPushButton::clicked, this, &WinPointOnSurface::onComputeCylinder);

    // 清空按钮
    m_btnClearCylinder = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnClearCylinder, &QPushButton::clicked, this, &WinPointOnSurface::onClear);

    // 结果标签
    m_labelResultCylinder = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("点坐标:"), pointInput);
    formLayout->addRow(QString::fromLocal8Bit("轴心坐标:"), axisInput);
    formLayout->addRow(QString::fromLocal8Bit("圆柱面参数:"), paramsInput);
    formLayout->addRow(methodLabel, m_comboMethodCylinder);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeCylinder);
    btnLayout->addWidget(m_btnClearCylinder);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultCylinder);
}

// 设置B样条曲面标签页
void WinPointOnSurface::setupBSplineTab(QWidget* tab)
{
    // 点输入框
    QWidget* pointInput = createPointInputLayout(m_pointX_bspline, m_pointY_bspline, m_pointZ_bspline);

    // B样条控制点输入框
    QWidget* controlPointsWidget = new QWidget;
    QHBoxLayout* controlLayout = new QHBoxLayout(controlPointsWidget);
    m_bsplineControlPoints = new QLineEdit("0,0,0; 10,0,5; 0,10,5; 10,10,10");
    controlLayout->addWidget(new QLabel(QString::fromLocal8Bit("控制点:")));
    controlLayout->addWidget(m_bsplineControlPoints);

    // 计算方法选择
    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethodBSpline = new QComboBox;
    m_comboMethodBSpline->addItem(QString::fromLocal8Bit("投影法  GeomAPI_ProjectPointOnSurf"), 0);
    m_comboMethodBSpline->addItem(QString::fromLocal8Bit("极值计算  Extrema_ExtPS"), 1);
    m_comboMethodBSpline->addItem(QString::fromLocal8Bit("面分析计算 ShapeAnalysis_Surface"), 2);
    m_comboMethodBSpline->addItem(QString::fromLocal8Bit("面域分类器  BRepClass_FaceClassifier"), 3);
    m_comboMethodBSpline->addItem(QString::fromLocal8Bit("Topo距离计算 BRepExtrema_DistShapeShape"), 4);

    // 计算按钮
    m_btnComputeBSpline = new QPushButton(QString::fromLocal8Bit("计算"));
    connect(m_btnComputeBSpline, &QPushButton::clicked, this, &WinPointOnSurface::onComputeBSpline);

    // 清空按钮
    m_btnClearBSpline = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnClearBSpline, &QPushButton::clicked, this, &WinPointOnSurface::onClear);

    // 结果标签
    m_labelResultBSpline = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("点坐标:"), pointInput);
    formLayout->addRow(QString::fromLocal8Bit("B样条控制点:"), controlPointsWidget);
    formLayout->addRow(methodLabel, m_comboMethodBSpline);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeBSpline);
    btnLayout->addWidget(m_btnClearBSpline);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultBSpline);
}




void WinPointOnSurface::onClear()
{
    for (const auto& obj : m_myObjects)
        m_context->Remove(obj, Standard_False);
    m_myObjects.clear();
    m_context->UpdateCurrentViewer();
}

bool WinPointOnSurface::parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip)
{
    bool xok, yok, zok;
    double x = xEdit->text().toDouble(&xok);
    double y = yEdit->text().toDouble(&yok);
    double z = zEdit->text().toDouble(&zok);
    if (!xok || !yok || !zok)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), tip + QString::fromLocal8Bit("坐标无效"));
        return false;
    }
    pt.SetCoord(x, y, z);
    return true;
}

bool WinPointOnSurface::parseDouble(QLineEdit* edit, double& value, const QString& tip)
{
    bool ok;
    value = edit->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), tip + QString::fromLocal8Bit("无效"));
        return false;
    }
    return true;
}

// 解析控制点字符串
std::vector<gp_Pnt> WinPointOnSurface::parseControlPoints(const QString& input)
{
    std::vector<gp_Pnt> points;
    QStringList pointList = input.split(";", Qt::SkipEmptyParts);

    for (const QString& pointStr : pointList) {
        QStringList coords = pointStr.split(",", Qt::SkipEmptyParts);
        if (coords.size() != 3) continue;

        bool xok, yok, zok;
        double x = coords[0].trimmed().toDouble(&xok);
        double y = coords[1].trimmed().toDouble(&yok);
        double z = coords[2].trimmed().toDouble(&zok);

        if (xok && yok && zok) {
            points.push_back(gp_Pnt(x, y, z));
        }
    }

    return points;
}




void WinPointOnSurface::drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name)
{
    Handle(AIS_Point) pt = new AIS_Point(new Geom_CartesianPoint(p));
    pt->SetColor(color);
    m_context->Display(pt, Standard_True);
    m_myObjects.append(pt);

    Handle(AIS_TextLabel) label = new AIS_TextLabel();
    label->SetPosition(p.Translated(gp_Vec(2, 2, 2)));
    label->SetText(TCollection_ExtendedString(name.toStdWString().c_str()));
    label->SetColor(color);
    m_context->Display(label, Standard_False);
    m_myObjects.append(label);
}

void WinPointOnSurface::drawPlane(const gp_Pnt& point, const gp_Dir& normal)
{
    gp_Pln plane(point, normal);
    gp_Dir uDir;
    if (std::abs(normal.Z()) > 0.9)
    {
        uDir = gp_Dir(1, 0, 0);
    }
    else
    {
        uDir = normal.Crossed(gp_Dir(0, 0, 1));
    }
    gp_Dir vDir = normal.Crossed(uDir);
    
    BRepBuilderAPI_MakePolygon poly;
    poly.Add(point.Translated(gp_Vec(uDir) * -500 + gp_Vec(vDir) * -500));
    poly.Add(point.Translated(gp_Vec(uDir) * 500 + gp_Vec(vDir) * -500));
    poly.Add(point.Translated(gp_Vec(uDir) * 500 + gp_Vec(vDir) * 500));
    poly.Add(point.Translated(gp_Vec(uDir) * -500 + gp_Vec(vDir) * 500));

    poly.Close();

    TopoDS_Face face = BRepBuilderAPI_MakeFace(poly.Wire());
    Handle(AIS_Shape) aisShape = new AIS_Shape(face);
    aisShape->SetTransparency(0.5);
    aisShape->SetColor(Quantity_NOC_BLUE1);
    m_context->Display(aisShape, Standard_True);
    m_myObjects.append(aisShape);
    m_myObjects.append(aisShape);
}
// 绘制球面 (增加可视化效果)
void WinPointOnSurface::drawSphere(const gp_Pnt& center, double radius)
{
    BRepPrimAPI_MakeSphere makeSphere(center, radius);
    makeSphere.Build();
    if (makeSphere.IsDone())
    {
        Handle(AIS_Shape) aisShape = new AIS_Shape(makeSphere.Shape());
        aisShape->SetColor(Quantity_NOC_RED);
        aisShape->SetTransparency(0.3);
        aisShape->SetDisplayMode(AIS_Shaded); // 使用着色模式
        m_context->Display(aisShape, Standard_True);
        m_myObjects.append(aisShape);

        // 添加球心点
        drawPoint(center, Quantity_NOC_BLACK, "Center");
    }
}

// 绘制圆柱面 (修复方向和尺寸问题)
void WinPointOnSurface::drawCylinder(const gp_Pnt& axisPoint, const gp_Dir& axisDir, double radius, double height)
{
    // 确保高度合理
    if (height < 1e-5) height = 100.0;

    gp_Ax2 cylinderAxis(axisPoint, axisDir);
    BRepPrimAPI_MakeCylinder makeCylinder(cylinderAxis, radius, height);
    makeCylinder.Build();
    if (makeCylinder.IsDone())
    {
        Handle(AIS_Shape) aisShape = new AIS_Shape(makeCylinder.Shape());
        aisShape->SetColor(Quantity_NOC_GREEN);
        aisShape->SetTransparency(0.3);
        aisShape->SetDisplayMode(AIS_Shaded);
        m_context->Display(aisShape, true);
        m_myObjects.append(aisShape);

        // 添加轴点
        drawPoint(axisPoint, Quantity_NOC_BLACK, "Axis");
    }
}

void WinPointOnSurface::drawBSplineSurface(const std::vector<gp_Pnt>& controlPoints)
{
    // 检查控制点数量
    if (controlPoints.size() != 4) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"),
            QString(QString::fromLocal8Bit("此函数仅支持4个控制点，当前数量: %1")).arg(controlPoints.size()));
        return;
    }

    // 构造控制点网格（2行2列）：
    // CP1 = 左下，CP2 = 右下，CP3 = 左上，CP4 = 右上
    TColgp_Array2OfPnt poles(1, 2, 1, 2);
    poles.SetValue(1, 1, controlPoints[0]); // 左下
    poles.SetValue(2, 1, controlPoints[1]); // 右下
    poles.SetValue(1, 2, controlPoints[2]); // 左上
    poles.SetValue(2, 2, controlPoints[3]); // 右上

    const int degreeU = 1;
    const int degreeV = 1;

    // U方向节点
    TColStd_Array1OfReal knotsU(1, 2);
    knotsU.SetValue(1, 0.0);
    knotsU.SetValue(2, 1.0);
    TColStd_Array1OfInteger multsU(1, 2);
    multsU.SetValue(1, 2);
    multsU.SetValue(2, 2);

    // V方向节点
    TColStd_Array1OfReal knotsV(1, 2);
    knotsV.SetValue(1, 0.0);
    knotsV.SetValue(2, 1.0);
    TColStd_Array1OfInteger multsV(1, 2);
    multsV.SetValue(1, 2);
    multsV.SetValue(2, 2);

    try {
        // 构造 B 样条曲面
        Handle(Geom_BSplineSurface) bsplineSurface = new Geom_BSplineSurface(
            poles, knotsU, knotsV, multsU, multsV, degreeU, degreeV,
            Standard_False, Standard_False
        );

        // 构造拓扑面
        TopoDS_Face face = BRepBuilderAPI_MakeFace(bsplineSurface, Precision::Confusion());

        // 显示曲面
        Handle(AIS_Shape) aisShape = new AIS_Shape(face);
        aisShape->SetColor(Quantity_NOC_YELLOW);
        aisShape->SetTransparency(0.4);
        m_context->Display(aisShape, Standard_True);
        m_myObjects.append(aisShape);

        // 显示控制点
        for (size_t i = 0; i < controlPoints.size(); ++i) {
            drawPoint(controlPoints[i], Quantity_NOC_MAGENTA,
                QString("CP%1").arg(i + 1));
        }
    }
    catch (Standard_Failure const& anException) {
        QString error = QString::fromLocal8Bit("创建B样条曲面失败: ");
        error += QString::fromUtf8(anException.GetMessageString());
        QMessageBox::critical(this, QString::fromLocal8Bit("错误"), error);
        if (m_outputFunc) {
            m_outputFunc(error);
        }
    }
}



void WinPointOnSurface::onComputePlane()
{
    onClear(); // 清空之前的图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_plane, m_pointY_plane, m_pointZ_plane, point, QString::fromLocal8Bit("点坐标"))) return;

    double a, b, c, d;
    if (!parseDouble(m_planeA, a, QString::fromLocal8Bit("平面参数A"))) return;
    if (!parseDouble(m_planeB, b, QString::fromLocal8Bit("平面参数B"))) return;
    if (!parseDouble(m_planeC, c, QString::fromLocal8Bit("平面参数C"))) return;
    if (!parseDouble(m_planeD, d, QString::fromLocal8Bit("平面参数D"))) return;

    // 获取选择的计算方法
    int method = m_comboMethodPlane->currentData().toInt();
    QString methodName = m_comboMethodPlane->currentText();

    // 绘制平面和点
    gp_Pnt planePoint(0, 0, d); // 平面上的一点（可选方式）
    gp_Dir normal(a, b, c);
    drawPlane(planePoint, normal);
    drawPoint(point, Quantity_NOC_GREEN, QString::fromLocal8Bit("点"));

    // --- 修改后的检查函数调用 ---
    bool isOnPlane = false;
    gp_Pnt nearPoint;
    double distance = 0.0;

    bool hasNear = checkPointOnPlane(point, a, b, c, d, method, isOnPlane, nearPoint, distance);

    // 绘制最近点（红色）和距离文字
    if (hasNear)
    {
        drawPoint(nearPoint, Quantity_NOC_RED, QString::fromLocal8Bit("最近点"));
    }

    // 合并显示文字
    QString baseResult = isOnPlane
        ? QString::fromLocal8Bit("点在平面上 (方法: %1)").arg(methodName)
        : QString::fromLocal8Bit("点不在平面上 (方法: %1)").arg(methodName);

    QString distInfo = hasNear
        ? QString::fromLocal8Bit("，最近距离: %1").arg(distance, 0, 'f', 6)
        : "";

    QString resultText = baseResult + distInfo;

    // 显示
    m_labelResultPlane->setText(resultText);
    if (m_outputFunc)
    {
        m_outputFunc(resultText);
    }
}

bool WinPointOnSurface::checkPointOnPlane(const gp_Pnt& point, double A, double B, double C, double D, int method, bool& isOnPlane, gp_Pnt& nearPoint, double& distance)
{
    isOnPlane = false;
    bool hasNearPoint = false;

    switch (method)
    {
    case METHOD_DistanceInterface:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByDistance(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByDistance(point, A, B, C, D, nearPoint, distance);
        break;
    case METHOD_Math:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByMath(point, A, B, C, D);
        break; // 没有最近点支持
    case METHOD_Projector:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByProject(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByProject(point, A, B, C, D, nearPoint, distance);
        break;
    case METHOD_AnalysisTool:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByShapeAnalysis(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByShapeAnalysis(point, A, B, C, D, nearPoint, distance);
        break;
    case METHOD_Extrema:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByExtrema(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByExt(point, A, B, C, D, nearPoint, distance);
        break;
    case METHOD_GeomAdaptor:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByGeomAdaptor(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByGeomSLProp(point, A, B, C, D, nearPoint, distance);
        break;
    case METHOD_FaceClassifier:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByFaceClassifier(point, A, B, C, D);
        break; // 没有最近点支持
    case METHOD_DistShapeShape:
        isOnPlane = op_pointPlaneRelationshipUtil->IsPointOnPlaneByDistShapeShape(point, A, B, C, D);
        hasNearPoint = op_pointPlaneRelationshipUtil->GetNearPointDistanceByDistShapeShape(point, A, B, C, D, nearPoint, distance);
        break;
    default:
        return false;
    }

    return hasNearPoint;
}

// 球面检查
void WinPointOnSurface::onComputeSphere()
{
    onClear(); // 清空之前的图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_sphere, m_pointY_sphere, m_pointZ_sphere, point, QString::fromLocal8Bit("点坐标"))) return;

    gp_Pnt sphereCenter;
    if (!parsePoint(m_sphereCenterX, m_sphereCenterY, m_sphereCenterZ, sphereCenter, QString::fromLocal8Bit("球心坐标"))) return;

    double sphereRadius;
    if (!parseDouble(m_sphereRadius, sphereRadius, QString::fromLocal8Bit("球面半径"))) return;

    // 获取选择的计算方法
    int method = m_comboMethodSphere->currentData().toInt();
    QString methodName = m_comboMethodSphere->currentText();

    // 绘制球面和点
    drawSphere(sphereCenter, sphereRadius);
    drawPoint(point, Quantity_NOC_BLUE, QString::fromLocal8Bit("点"));

    // 检查是否在球面上，并获取最近点和距离
    bool isOnSphere = false;
    gp_Pnt nearPoint;
    double distance = 0.0;

    bool hasNear = checkPointOnSphere(point, sphereCenter, sphereRadius, method, isOnSphere, nearPoint, distance);

    // 绘制最近点
    if (hasNear)
    {
        drawPoint(nearPoint, Quantity_NOC_RED, QString::fromLocal8Bit("最近点"));
    }

    // 构造显示文本
    QString baseResult = isOnSphere
        ? QString::fromLocal8Bit("点在球面上 (方法: %1)").arg(methodName)
        : QString::fromLocal8Bit("点不在球面上 (方法: %1)").arg(methodName);

    QString distInfo = hasNear
        ? QString::fromLocal8Bit("，最近距离: %1").arg(distance, 0, 'f', 6)
        : "";

    QString resultText = baseResult + distInfo;

    m_labelResultSphere->setText(resultText);
    if (m_outputFunc)
    {
        m_outputFunc(resultText);
    }
}

// 检查点是否在球面上
bool WinPointOnSurface::checkPointOnSphere(const gp_Pnt& point, const gp_Pnt& center, double radius, int method, bool& isOnSphere, gp_Pnt& nearPoint, double& distance)
{
    isOnSphere = false;
    bool hasNear = false;

    switch (method)
    {
    case 0: // Distance
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByDistance(point, center, radius);
        hasNear = op_pointSurfaceRelationUtil->GetSphereNearPointDistanceByDistance(point, center, radius, nearPoint, distance);
        break;
    case 1: // Project
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByProject(point, center, radius);
        hasNear = op_pointSurfaceRelationUtil->GetSphereNearPointDistanceByProject(point, center, radius, nearPoint, distance);
        break;
    case 2: // Extrema
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByExtrema(point, center, radius);
        hasNear = op_pointSurfaceRelationUtil->GetSphereNearPointDistanceByExtrema(point, center, radius, nearPoint, distance);
        break;
    case 3: // ShapeAnalysis
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByShapeAnalysis(point, center, radius);
        hasNear = op_pointSurfaceRelationUtil->GetSphereNearPointDistanceByAnalysis(point, center, radius, nearPoint, distance);
        break;
    case 4: // FaceClassifier
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByFaceClassifier(point, center, radius);
        break; // 不支持最近点
    case 5: // DistShapeShape
        isOnSphere = op_pointSurfaceRelationUtil->OnSphereByDistShapeShape(point, center, radius);
        hasNear = op_pointSurfaceRelationUtil->GetSphereNearPointDistanceByDistShapeShape(point, center, radius, nearPoint, distance);
        break;
    default:
        return false;
    }

    return hasNear;
}


// 圆柱面检查
void WinPointOnSurface::onComputeCylinder()
{
    onClear(); // 清空之前图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_cylinder, m_pointY_cylinder, m_pointZ_cylinder, point, QString::fromLocal8Bit("点坐标")))
        return;

    gp_Pnt cylinderAxisPoint;
    if (!parsePoint(m_cylinderAxisX, m_cylinderAxisY, m_cylinderAxisZ, cylinderAxisPoint, QString::fromLocal8Bit("圆柱轴心坐标")))
        return;

    double cylinderRadius, cylinderHeight;
    if (!parseDouble(m_cylinderRadius, cylinderRadius, QString::fromLocal8Bit("圆柱面半径"))) return;
    if (!parseDouble(m_cylinderHeight, cylinderHeight, QString::fromLocal8Bit("圆柱面高度"))) return;

    // 获取计算方法
    int method = m_comboMethodSphere->currentData().toInt(); // 注意是否使用独立下拉框 m_comboMethodCylinder
    QString methodName = m_comboMethodSphere->currentText();

    // 绘制圆柱面和点
    drawCylinder(cylinderAxisPoint, gp_Dir(0, 0, 1), cylinderRadius, cylinderHeight);
    drawPoint(point, Quantity_NOC_RED, QString::fromLocal8Bit("点"));

    // 调用统一接口
    bool isOnCylinder = false;
    gp_Pnt nearPoint;
    double distance = 0.0;
    bool hasNear = checkPointOnCylinder(point, cylinderAxisPoint, gp_Dir(0, 0, 1), cylinderRadius, method, isOnCylinder, nearPoint, distance);

    if (hasNear)
    {
        drawPoint(nearPoint, Quantity_NOC_GREEN, QString::fromLocal8Bit("最近点"));
    }

    QString baseResult = isOnCylinder
        ? QString::fromLocal8Bit("点在圆柱面上 (方法: %1)").arg(methodName)
        : QString::fromLocal8Bit("点不在圆柱面上 (方法: %1)").arg(methodName);

    QString distInfo = hasNear
        ? QString::fromLocal8Bit("，最近距离: %1").arg(distance, 0, 'f', 6)
        : "";

    QString resultText = baseResult + distInfo;

    m_labelResultCylinder->setText(resultText);
    if (m_outputFunc)
    {
        m_outputFunc(resultText);
    }
}

bool WinPointOnSurface::checkPointOnCylinder(const gp_Pnt& point, const gp_Pnt& axisPoint, const gp_Dir& axisDir,
    double radius, int method, bool& isOnCylinder, gp_Pnt& nearPoint, double& distance)
{
    isOnCylinder = false;
    bool hasNear = false;

    switch (method)
    {
    case 0: // Distance
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByDistance(point, axisPoint, axisDir, radius);
        hasNear = op_pointSurfaceRelationUtil->GetCylinderNearPointDisByDistance(point, axisPoint, axisDir, radius, nearPoint, distance);
        break;
    case 1: // Project
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByProject(point, axisPoint, axisDir, radius);
        hasNear = op_pointSurfaceRelationUtil->GetCylinderNearPointDisByProject(point, axisPoint, axisDir, radius, nearPoint, distance);
        break;
    case 2: // Extrema
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByExtrema(point, axisPoint, axisDir, radius);
        hasNear = op_pointSurfaceRelationUtil->GetCylinderNearPointDisByExtrema(point, axisPoint, axisDir, radius, nearPoint, distance);
        break;
    case 3: // ShapeAnalysis
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByShapeAnalysis(point, axisPoint, axisDir, radius);
        hasNear = op_pointSurfaceRelationUtil->GetCylinderNearPointDisByAnalysis(point, axisPoint, axisDir, radius, nearPoint, distance);
        break;
    case 4: // FaceClassifier
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByFaceClassifier(point, axisPoint, axisDir, radius);
        break; // 不支持最近点
    case 5: // DistShapeShape
        isOnCylinder = op_pointSurfaceRelationUtil->OnCylinderByDistShapeShape(point, axisPoint, axisDir, radius);
        hasNear = op_pointSurfaceRelationUtil->GetCylinderNearPointDisByDistShape(point, axisPoint, axisDir, radius, nearPoint, distance);
        break;
    default:
        return false;
    }

    return hasNear;
}




// B样条曲面检查
void WinPointOnSurface::onComputeBSpline()
{
    onClear(); // 清空之前的图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_bspline, m_pointY_bspline, m_pointZ_bspline, point, QString::fromLocal8Bit("点坐标")))
        return;

    // 解析控制点
    std::vector<gp_Pnt> controlPoints = parseControlPoints(m_bsplineControlPoints->text());
    if (controlPoints.empty()) {
        QMessageBox::warning(this, "错误", "无法解析控制点");
        return;
    }

    // 获取计算方法
    int method = m_comboMethodSphere->currentData().toInt();  // 建议改为 m_comboMethodBSpline
    QString methodName = m_comboMethodSphere->currentText();

    // 绘制B样条面和点
    drawBSplineSurface(controlPoints);
    drawPoint(point, Quantity_NOC_GREEN, QString::fromLocal8Bit("点"));

    // 判断 + 获取最近点和距离
    bool isOnBSpline = false;
    gp_Pnt nearPoint;
    double distance = 0.0;
    bool hasNear = checkPointOnBSplineSurface(point, controlPoints, method, isOnBSpline, nearPoint, distance);

    // 绘制最近点
    if (hasNear)
    {
        drawPoint(nearPoint, Quantity_NOC_RED, QString::fromLocal8Bit("最近点"));
    }

    QString baseResult = isOnBSpline
        ? QString::fromLocal8Bit("点在B样条曲面上 (方法: %1)").arg(methodName)
        : QString::fromLocal8Bit("点不在B样条曲面上 (方法: %1)").arg(methodName);

    QString distInfo = hasNear
        ? QString::fromLocal8Bit("，最近距离: %1").arg(distance, 0, 'f', 6)
        : "";

    QString resultText = baseResult + distInfo;

    m_labelResultBSpline->setText(resultText);
    if (m_outputFunc)
    {
        m_outputFunc(resultText);
    }
}

// 检查点是否在B样条曲面上
bool WinPointOnSurface::checkPointOnBSplineSurface(const gp_Pnt& point, const std::vector<gp_Pnt>& controlPoints, int method,
    bool& isOnBSpline, gp_Pnt& nearPoint, double& distance)
{
    isOnBSpline = false;
    bool hasNear = false;

    switch (method)
    {
    case 0: // 投影
        isOnBSpline = op_pointSurfaceRelationUtil->OnBSplineByProjection(point, controlPoints);
        hasNear = op_pointSurfaceRelationUtil->GetBSplineNearPointDisByProject(point, controlPoints, nearPoint, distance);
        break;
    case 1: // 极值
        isOnBSpline = op_pointSurfaceRelationUtil->OnBSplineByExtrema(point, controlPoints);
        hasNear = op_pointSurfaceRelationUtil->GetBSplineNearPointDisByExtrema(point, controlPoints, nearPoint, distance);
        break;
    case 2: // ShapeAnalysis
        isOnBSpline = op_pointSurfaceRelationUtil->OnBSplineByShapeAnalysis(point, controlPoints);
        hasNear = op_pointSurfaceRelationUtil->GetBSplineNearPointDisByShapeAnalysis(point, controlPoints, nearPoint, distance);
        break;
    case 3: // FaceClassifier
        isOnBSpline = op_pointSurfaceRelationUtil->OnBSplineByFaceClassifier(point, controlPoints);
        break; // 不支持最近点
    case 4: // DistShapeShape
        isOnBSpline = op_pointSurfaceRelationUtil->OnBSplineByDistShapeShape(point, controlPoints);
        hasNear = op_pointSurfaceRelationUtil->GetBSplineNearPointDisByDistShapeShape(point, controlPoints, nearPoint, distance);
        break;
    default:
        return false;
    }

    return hasNear;
}















