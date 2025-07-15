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

// 容差值
const double Tolerance = Precision::Confusion();

WinPointOnSurface::WinPointOnSurface(Handle(AIS_InteractiveContext) context,
    Handle(V3d_Viewer) v3dViewer,
    Handle(V3d_View) v3dView,
    QWidget* parent,
    std::function<void(const QString&)> func)
    : QDialog(parent), m_context(context), m_v3dView(v3dView),
    m_v3dViewer(v3dViewer), m_outputFunc(func)
{
    setupUI();
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
    m_bsplineControlPoints = new QLineEdit("0,0,0; 10,0,0; 10,10,0; 0,10,0");
    controlLayout->addWidget(new QLabel(QString::fromLocal8Bit("控制点:")));
    controlLayout->addWidget(m_bsplineControlPoints);

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



}



// 平面检查
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

    // 绘制平面和点
    gp_Pnt planePoint(0, 0, d); // 平面上的一个点
    gp_Dir normal(a, b, c); // 法向量
    drawPlane(planePoint, normal);
    drawPoint(point, Quantity_NOC_GREEN, QString::fromLocal8Bit("点"));

    // 检查点是否在平面上
    bool isOnPlane = checkPointOnPlane(point, a, b, c, d);
    if (isOnPlane)
    {
        m_labelResultPlane->setText(QString::fromLocal8Bit("点在平面上"));
    }
    else
    {
        m_labelResultPlane->setText(QString::fromLocal8Bit("点不在平面上"));
    }
}

// 球面检查
void WinPointOnSurface::onComputeSphere()
{
    onClear(); // 清空之前的图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_sphere, m_pointY_sphere, m_pointZ_sphere, point, QString::fromLocal8Bit("点坐标"))) return;

    gp_Pnt sphereCenter;
    if (!parsePoint(m_sphereCenterX, m_sphereCenterY, m_sphereCenterZ, sphereCenter, QString::fromLocal8Bit("球心坐标")))
        return;

    double sphereRadius;
    if (!parseDouble(m_sphereRadius, sphereRadius, QString::fromLocal8Bit("球面半径"))) return;

    // 绘制球面和点
    drawSphere(sphereCenter, sphereRadius);
    drawPoint(point, Quantity_NOC_BLUE, QString::fromLocal8Bit("点"));

    bool isOnSphere = checkPointOnSphere(point, sphereCenter, sphereRadius);
    if (isOnSphere)
    {
        m_labelResultSphere->setText(QString::fromLocal8Bit("点在球面上"));
    }
    else
    {
        m_labelResultSphere->setText(QString::fromLocal8Bit("点不在球面上"));
    }
}

// 圆柱面检查
void WinPointOnSurface::onComputeCylinder()
{
    onClear(); // 清空之前的图形

    gp_Pnt point;
    if (!parsePoint(m_pointX_cylinder, m_pointY_cylinder, m_pointZ_cylinder, point, QString::fromLocal8Bit("点坐标")))
        return;

    gp_Pnt cylinderAxisPoint;
    if (!parsePoint(m_cylinderAxisX, m_cylinderAxisY, m_cylinderAxisZ, cylinderAxisPoint, QString::fromLocal8Bit("圆柱轴心坐标")))
        return;

    double cylinderRadius, cylinderHeight;
    if (!parseDouble(m_cylinderRadius, cylinderRadius, QString::fromLocal8Bit("圆柱面半径"))) return;
    if (!parseDouble(m_cylinderHeight, cylinderHeight, QString::fromLocal8Bit("圆柱面高度"))) return;

    // 绘制圆柱面和点
    drawCylinder(cylinderAxisPoint, gp_Dir(0, 0, 1), cylinderRadius, cylinderHeight);
    drawPoint(point, Quantity_NOC_RED, QString::fromLocal8Bit("点"));

    bool isOnCylinder = checkPointOnCylinder(point, cylinderAxisPoint, gp_Dir(0, 0, 1), cylinderRadius);
    if (isOnCylinder)
    {
        m_labelResultCylinder->setText(QString::fromLocal8Bit("点在圆柱面上"));
    }
    else
    {
        m_labelResultCylinder->setText(QString::fromLocal8Bit("点不在圆柱面上"));
    }
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

    // 绘制B样条曲面和点
    drawBSplineSurface(controlPoints);
    drawPoint(point, Quantity_NOC_MAGENTA, QString::fromLocal8Bit("点"));

    bool isOnBSpline = checkPointOnBSplineSurface(point, controlPoints);
    if (isOnBSpline)
    {
        m_labelResultBSpline->setText(QString::fromLocal8Bit("点在B样条曲面上"));
    }
    else {
        m_labelResultBSpline->setText(QString::fromLocal8Bit("点不在B样条曲面上"));
    }
}

// 检查点是否在平面上
bool WinPointOnSurface::checkPointOnPlane(const gp_Pnt& point, double A, double B, double C, double D)
{
    // 创建平面几何体
    gp_Pln plane(A, B, C, D);

    // 计算点到平面的距离
    double distance = plane.Distance(point);

    // 距离小于容差则认为点在平面上
    return distance <= Tolerance;
}

// 检查点是否在球面上
bool WinPointOnSurface::checkPointOnSphere(const gp_Pnt& point, const gp_Pnt& center, double radius)
{
    // 计算点到球心的距离
    double distance = point.Distance(center);

    // 计算距离与半径的差值
    double diff = std::abs(distance - radius);

    // 差值小于容差则认为点在球面上
    return diff <= Tolerance;
}

// 检查点是否在圆柱面上
bool WinPointOnSurface::checkPointOnCylinder(const gp_Pnt& point, const gp_Pnt& axisPoint, const gp_Dir& axisDir, double radius)
{
    // 创建圆柱轴线的方向向量
    gp_Vec axisVec(axisDir);

    // 计算点到轴线的向量
    gp_Vec pointToAxis(axisPoint, point);

    // 计算点到轴线的投影长度
    double projectionLength = pointToAxis.Dot(axisVec);

    // 计算投影点坐标
    gp_Pnt projectionPoint = axisPoint.Translated(axisVec * projectionLength);

    // 计算点到轴线的垂直距离
    double distance = point.Distance(projectionPoint);

    // 计算距离与半径的差值
    double diff = std::abs(distance - radius);

    // 差值小于容差则认为点在圆柱面上
    return diff <= Tolerance;
}

// 检查点是否在B样条曲面上
bool WinPointOnSurface::checkPointOnBSplineSurface(const gp_Pnt& point, const std::vector<gp_Pnt>& controlPoints)
{
    // 检查控制点数量是否足够
    if (controlPoints.size() < 4) {
        return false;
    }

    // 确定网格尺寸（简单实现：2x2网格）
    const int numU = 2;
    const int numV = 2;

    // 创建二维控制点数组
    TColgp_Array2OfPnt poles(1, numU, 1, numV);

    // 将一维数组转换为二维网格
    for (int u = 1; u <= numU; ++u) {
        for (int v = 1; v <= numV; ++v) {
            int index = (u - 1) * numV + (v - 1);
            if (index < static_cast<int>(controlPoints.size())) {
                poles.SetValue(u, v, controlPoints[index]);
            }
        }
    }

    // 创建均匀参数化的节点序列
    TColStd_Array1OfReal knotsU(1, 2);
    knotsU.SetValue(1, 0.0);
    knotsU.SetValue(2, 1.0);

    TColStd_Array1OfReal knotsV(1, 2);
    knotsV.SetValue(1, 0.0);
    knotsV.SetValue(2, 1.0);

    // 创建重数数组（两端全重数）
    TColStd_Array1OfInteger multsU(1, 2);
    multsU.SetValue(1, numU);
    multsU.SetValue(2, numU);

    TColStd_Array1OfInteger multsV(1, 2);
    multsV.SetValue(1, numV);
    multsV.SetValue(2, numV);

    // 创建B样条曲面（线性B样条）
    const int degreeU = 1;
    const int degreeV = 1;

    Handle(Geom_BSplineSurface) bsplineSurface = new Geom_BSplineSurface(
        poles,
        knotsU, knotsV,
        multsU, multsV,
        degreeU, degreeV,
        Standard_False, Standard_False
    );

    // 将几何曲面转换为拓扑形状
    TopoDS_Face face = BRepBuilderAPI_MakeFace(bsplineSurface, Tolerance);

    // 创建点对应的拓扑顶点
    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(point);

    // 计算点到曲面的最小距离
    BRepExtrema_DistShapeShape distCalculator;
    distCalculator.LoadS1(vertex);
    distCalculator.LoadS2(face);
    distCalculator.Perform();

    // 检查是否计算成功且距离在容差范围内
    return distCalculator.IsDone() &&
        distCalculator.Value() <= Tolerance;
}