#include "WinSegmentBSplineDistance.h"
#include "GeometryUtilityOccDefine.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>

#include "JNew.h"








WinSegmentBSplineDistance::WinSegmentBSplineDistance(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView, QWidget* parent,  std::function<void(const QString&)> outputFunc)
    : QDialog(parent), m_context(std::move(context)), m_v3dViewer(std::move(v3dViewer)), m_v3dView(std::move(v3dView)), m_outputFunc(std::move(outputFunc))
{
    setupUI();
    m_opUti = NEW_AS_OWNER_PTR(GeomLineSegmentRelationUtility);
}

void WinSegmentBSplineDistance::setupUI()
{
    setWindowTitle(QString::fromLocal8Bit("线段与B样条最近距离计算"));
    setMinimumSize(600, 400);

    QWidget* p1Widget = createPointInput(QString::fromLocal8Bit("线段起点"), m_p1x, m_p1y, m_p1z, "0", "0", "0");
    QWidget* p2Widget = createPointInput(QString::fromLocal8Bit("线段终点"), m_p2x, m_p2y, m_p2z, "100", "0", "0");
    QWidget* c1Widget = createPointInput(QString::fromLocal8Bit("控制点1"), m_c1x, m_c1y, m_c1z, "0", "0", "0");
    QWidget* c2Widget = createPointInput(QString::fromLocal8Bit("控制点2"), m_c2x, m_c2y, m_c2z, "50", "0", "0");
    QWidget* c3Widget = createPointInput(QString::fromLocal8Bit("控制点3"), m_c3x, m_c3y, m_c3z, "50", "50", "0");
    QWidget* c4Widget = createPointInput(QString::fromLocal8Bit("控制点4"), m_c4x, m_c4y, m_c4z, "0", "50", "0");

    m_btnCompute = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClear = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnCompute, &QPushButton::clicked, this, &WinSegmentBSplineDistance::onCompute);
    connect(m_btnClear, &QPushButton::clicked, this, &WinSegmentBSplineDistance::onClear);
    m_labelResult = new QLabel;

    QFormLayout* form = new QFormLayout;
    form->addRow(QString::fromLocal8Bit("线段起点 (x,y,z):"), p1Widget);
    form->addRow(QString::fromLocal8Bit("线段终点 (x,y,z):"), p2Widget);
    form->addRow(QString::fromLocal8Bit("控制点1 (x,y,z):"), c1Widget);
    form->addRow(QString::fromLocal8Bit("控制点2 (x,y,z):"), c2Widget);
    form->addRow(QString::fromLocal8Bit("控制点3 (x,y,z):"), c3Widget);
    form->addRow(QString::fromLocal8Bit("控制点4 (x,y,z):"), c4Widget);

    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethod = new QComboBox;
    m_comboMethod->addItem(QString::fromLocal8Bit("极值法 (GeomAPI_ExtremaCurveCurve)"), METHOD_ExtremaCurveCurve);
    m_comboMethod->addItem(QString::fromLocal8Bit("Topo距离 (BRepExtrema_DistShapeShape)"), METHOD_DistShapeShape);
    m_comboMethod->addItem(QString::fromLocal8Bit("端点投影 (GeomAPI_ProjectPointOnCurve)"), METHOD_ProjectPointOnCurve);
    m_comboMethod->addItem(QString::fromLocal8Bit("端点分析投影 (ShapeAnalysis_Curve::Project)"), METHOD_ShapeAnalysisProject);
    form->addRow(methodLabel, m_comboMethod);

    QHBoxLayout* buttons = new QHBoxLayout;
    buttons->addWidget(m_btnCompute);
    buttons->addWidget(m_btnClear);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(form);
    mainLayout->addLayout(buttons);
    mainLayout->addWidget(m_labelResult);
    setLayout(mainLayout);
}

QWidget* WinSegmentBSplineDistance::createPointInput(const QString& labelText, QLineEdit*& px, QLineEdit*& py, QLineEdit*& pz,
    const QString& dx, const QString& dy, const QString& dz)
{
    px = new QLineEdit(dx);
    py = new QLineEdit(dy);
    pz = new QLineEdit(dz);

    QWidget* row = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(row);
    layout->addWidget(new QLabel("X:"));
    layout->addWidget(px);
    layout->addWidget(new QLabel("Y:"));
    layout->addWidget(py);
    layout->addWidget(new QLabel("Z:"));
    layout->addWidget(pz);
    layout->setContentsMargins(0, 0, 0, 0);
    return row;
}


bool WinSegmentBSplineDistance::parsePoint(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez, gp_Pnt& p, const QString& tip)
{
    bool okx, oky, okz;
    double x = ex->text().toDouble(&okx);
    double y = ey->text().toDouble(&oky);
    double z = ez->text().toDouble(&okz);
    if (!okx || !oky || !okz) {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), tip + QString::fromLocal8Bit("坐标无效"));
        return false;
    }
    p.SetCoord(x, y, z);
    return true;
}

void WinSegmentBSplineDistance::onClear()
{
    for (const auto& obj : m_objects) {
        m_context->Remove(obj, Standard_False);
    }
    m_objects.clear();
    m_context->UpdateCurrentViewer();
    m_labelResult->clear();
}

void WinSegmentBSplineDistance::onCompute()
{
    onClear();

    gp_Pnt p1, p2;
    if (!parsePoint(m_p1x, m_p1y, m_p1z, p1, QString::fromLocal8Bit("线段起点"))) return;
    if (!parsePoint(m_p2x, m_p2y, m_p2z, p2, QString::fromLocal8Bit("线段终点"))) return;

    std::vector<gp_Pnt> ctrl(4);
    if (!parsePoint(m_c1x, m_c1y, m_c1z, ctrl[0], QString::fromLocal8Bit("控制点1"))) return;
    if (!parsePoint(m_c2x, m_c2y, m_c2z, ctrl[1], QString::fromLocal8Bit("控制点2"))) return;
    if (!parsePoint(m_c3x, m_c3y, m_c3z, ctrl[2], QString::fromLocal8Bit("控制点3"))) return;
    if (!parsePoint(m_c4x, m_c4y, m_c4z, ctrl[3], QString::fromLocal8Bit("控制点4"))) return;

    drawSegment(p1, p2, Quantity_NOC_BLUE1);
    drawBSpline(ctrl);

    int methodVal = 0;
    if (m_comboMethod) {
        QVariant data = m_comboMethod->currentData();
        methodVal = data.isValid() ? data.toInt() : 0;
    }

    gp_Pnt nearOnLine, nearOnSpline;
    computeNearest(p1, p2, ctrl, nearOnLine, nearOnSpline, methodVal);

    drawPoint(nearOnLine, Quantity_NOC_RED, QString::fromLocal8Bit("最近点(直线)"));
    drawPoint(nearOnSpline, Quantity_NOC_RED, QString::fromLocal8Bit("最近点(B样条)"));
    drawConnectingSegment(nearOnLine, nearOnSpline);

    double dist = nearOnLine.Distance(nearOnSpline);
    QString info = QString::fromLocal8Bit("最近距离: %1\n最近点1: (%2, %3, %4)\n最近点2: (%5, %6, %7)")
        .arg(dist, 0, 'f', 6)
        .arg(nearOnLine.X(), 0, 'f', 3)
        .arg(nearOnLine.Y(), 0, 'f', 3)
        .arg(nearOnLine.Z(), 0, 'f', 3)
        .arg(nearOnSpline.X(), 0, 'f', 3)
        .arg(nearOnSpline.Y(), 0, 'f', 3)
        .arg(nearOnSpline.Z(), 0, 'f', 3);
    m_labelResult->setText(info);
    if (m_outputFunc) {
        m_outputFunc(info);
    }
    m_context->UpdateCurrentViewer();
}

void WinSegmentBSplineDistance::drawSegment(const gp_Pnt& p1, const gp_Pnt& p2, Quantity_NameOfColor col)
{
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);
    Handle(AIS_Shape) shape = new AIS_Shape(edge);
    shape->SetColor(col);
    shape->SetWidth(2.0);
    m_context->Display(shape, Standard_True);
    m_objects.append(shape);
}


void WinSegmentBSplineDistance::drawBSpline(const std::vector<gp_Pnt>& controlPoints)
{
    if (controlPoints.size() < 2) {
        return;
    }
    TColgp_Array1OfPnt poles(1, static_cast<int>(controlPoints.size()));
    for (Standard_Integer i = 1; i <= controlPoints.size(); ++i) {
        poles.SetValue(i, controlPoints[i - 1]);
    }
    Handle(Geom_BSplineCurve) bspline = GeomAPI_PointsToBSpline(poles);
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(bspline);
    Handle(AIS_Shape) shape = new AIS_Shape(edge);
    shape->SetColor(Quantity_NOC_GREEN1);
    shape->SetWidth(2.0);
    m_context->Display(shape, Standard_True);
    m_objects.append(shape);
}


void WinSegmentBSplineDistance::drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name)
{
    Handle(AIS_Point) point = new AIS_Point(new Geom_CartesianPoint(p));
    point->SetColor(color);
    m_context->Display(point, Standard_True);
    m_objects.append(point);
    // Add a text label slightly offset from the point
    Handle(AIS_TextLabel) label = new AIS_TextLabel();
    label->SetPosition(p.Translated(gp_Vec(2, 2, 2)));
    label->SetText(TCollection_ExtendedString(name.toStdWString().c_str()));
    label->SetColor(color);
    m_context->Display(label, Standard_False);
    m_objects.append(label);
}


void WinSegmentBSplineDistance::drawConnectingSegment(const gp_Pnt& p1, const gp_Pnt& p2)
{
    if (p1.Distance(p2) < 1)
        return;

    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);
    Handle(AIS_Shape) shape = new AIS_Shape(edge);
    shape->SetColor(Quantity_NOC_RED);
    shape->SetWidth(2.0);
    m_context->Display(shape, Standard_False);
    m_objects.append(shape);
}

void WinSegmentBSplineDistance::computeNearest(const gp_Pnt& p1, const gp_Pnt& p2, const std::vector<gp_Pnt> & ctrlPts,  gp_Pnt& nearOnLine,gp_Pnt& nearOnSpline,  int method)
{

    gp_Vec vec(p1, p2);
    if (vec.Magnitude() < Precision::Confusion() || ctrlPts.size() < 1) {
        nearOnLine = p1;
        nearOnSpline = ctrlPts.empty() ? p1 : ctrlPts.front();
        return;
    }

    // Working variables to hold potential nearest points
    gp_Pnt candidateLine, candidateSpline;
    bool ok = false;
    switch (static_cast<Method>(method)) {
    case METHOD_ExtremaCurveCurve:
        ok = m_opUti->DistanceBSplineUsingExtremaCurveCurve(p1, p2, ctrlPts, candidateLine, candidateSpline);
        break;
    case METHOD_DistShapeShape:
        ok = m_opUti->DistanceBSplineUsingDistShapeShape(p1, p2, ctrlPts, candidateLine, candidateSpline);
        break;
    case METHOD_ProjectPointOnCurve:
        ok = m_opUti->DistanceBSplineUsingProjectPointOnCurve(p1, p2, ctrlPts, candidateLine, candidateSpline);
        break;
    case METHOD_ShapeAnalysisProject:
        ok = m_opUti->DistanceBSplineUsingShapeAnalysisProject(p1, p2, ctrlPts, candidateLine, candidateSpline);
        break;
    default:
        ok = false;
        break;
    }

    if (ok) 
    {
        nearOnLine = candidateLine;
        nearOnSpline = candidateSpline;
        return;
    }
}