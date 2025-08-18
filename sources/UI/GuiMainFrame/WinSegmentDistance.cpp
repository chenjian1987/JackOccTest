#include "WinSegmentDistance.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>

#include <AIS_Shape.hxx>
#include <AIS_Point.hxx>
#include <AIS_TextLabel.hxx>
#include <Geom_CartesianPoint.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <Quantity_Color.hxx>

#include "JNew.h"



// Constructor
WinSegmentDistance::WinSegmentDistance(Handle(AIS_InteractiveContext) context,Handle(V3d_Viewer) v3dViewer,Handle(V3d_View) v3dView, QWidget* parent,std::function<void(const QString&)> outputFunc)
    : QDialog(parent),
    m_context(std::move(context)),m_v3dViewer(std::move(v3dViewer)),
    m_v3dView(std::move(v3dView)),
    m_outputFunc(std::move(outputFunc))
{
    setupUI();
    m_opUti = NEW_AS_OWNER_PTR(GeomLineSegmentRelationUtility);
}


void WinSegmentDistance::setupUI()
{
    setWindowTitle(QString::fromLocal8Bit("线段最近距离计算"));
    setMinimumSize(500, 350);

    // Create input layouts for P1, P2, Q1, Q2
    QWidget* p1Input = createPointInputLayout(m_p1x, m_p1y, m_p1z);
    QWidget* p2Input = createPointInputLayout(m_p2x, m_p2y, m_p2z);
    QWidget* q1Input = createPointInputLayout(m_q1x, m_q1y, m_q1z);
    QWidget* q2Input = createPointInputLayout(m_q2x, m_q2y, m_q2z);

    // Method selection combo box
    QLabel* methodLabel = new QLabel(QString::fromLocal8Bit("计算方法:"));
    m_comboMethod = new QComboBox;
    // Add algorithm choices with userData equal to the Method enum values
    m_comboMethod->addItem(QString::fromLocal8Bit("极值法 (GeomAPI_ExtremaCurveCurve)"), METHOD_ExtremaCurve);
    m_comboMethod->addItem(QString::fromLocal8Bit("Topo距离 (BRepExtrema_DistShapeShape)"), METHOD_DistShapeShape);
    m_comboMethod->addItem(QString::fromLocal8Bit("投影法 (GeomAPI_ProjectPointOnCurve)"), METHOD_ProjectPointOnCurve);
    m_comboMethod->addItem(QString::fromLocal8Bit("分析投影 (ShapeAnalysis_Curve::Project)"), METHOD_ShapeAnalysisProject);
    m_comboMethod->addItem(QString::fromLocal8Bit("手工矢量算法"), METHOD_Manual);

    m_btnCompute = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClear = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnCompute, &QPushButton::clicked, this, &WinSegmentDistance::onCompute);
    connect(m_btnClear, &QPushButton::clicked, this, &WinSegmentDistance::onClear);
    m_labelResult = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("P1 (x,y,z):"), p1Input);
    formLayout->addRow(QString::fromLocal8Bit("P2 (x,y,z):"), p2Input);
    formLayout->addRow(QString::fromLocal8Bit("Q1 (x,y,z):"), q1Input);
    formLayout->addRow(QString::fromLocal8Bit("Q2 (x,y,z):"), q2Input);
    formLayout->addRow(methodLabel, m_comboMethod);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnCompute);
    btnLayout->addWidget(m_btnClear);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResult);
    setLayout(mainLayout);
}

QWidget* WinSegmentDistance::createPointInputLayout(QLineEdit*& x, QLineEdit*& y, QLineEdit*& z)
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

void WinSegmentDistance::onClear()
{
    for (const auto& obj : m_myObjects) {
        m_context->Remove(obj, Standard_False);
    }
    m_myObjects.clear();
    m_context->UpdateCurrentViewer();
    m_labelResult->clear();
}



bool WinSegmentDistance::parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit,
    gp_Pnt& pt, const QString& tip)
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


void WinSegmentDistance::drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name)
{
    Handle(AIS_Point) pt = new AIS_Point(new Geom_CartesianPoint(p));
    pt->SetColor(color);
    m_context->Display(pt, Standard_True);
    m_myObjects.append(pt);

    Handle(AIS_TextLabel) label = new AIS_TextLabel();
    // Offset the label slightly to avoid overlapping the point
    label->SetPosition(p.Translated(gp_Vec(2, 2, 2)));
    label->SetText(TCollection_ExtendedString(name.toStdWString().c_str()));
    label->SetColor(color);
    m_context->Display(label, Standard_False);
    m_myObjects.append(label);
}



void WinSegmentDistance::drawSegment(const gp_Pnt& p1, const gp_Pnt& p2, Quantity_NameOfColor color)
{
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);
    Handle(AIS_Shape) aisShape = new AIS_Shape(edge);
    aisShape->SetColor(color);
    // Set line width for wireframe presentation
    aisShape->SetWidth(2.0);
    m_context->Display(aisShape, Standard_True);
    m_myObjects.append(aisShape);
}


void WinSegmentDistance::drawLabel(const gp_Pnt& p, const QString& text, Quantity_NameOfColor color)
{
    Handle(AIS_TextLabel) label = new AIS_TextLabel();
    label->SetPosition(p.Translated(gp_Vec(3, 3, 3)));
    label->SetText(TCollection_ExtendedString(text.toStdWString().c_str()));
    label->SetColor(color);
    m_context->Display(label, Standard_True);
    m_myObjects.append(label);
}


void WinSegmentDistance::onCompute()
{
    onClear();
    gp_Pnt p1, p2, q1, q2;
    if (!parsePoint(m_p1x, m_p1y, m_p1z, p1, QString::fromLocal8Bit("P1"))) return;
    if (!parsePoint(m_p2x, m_p2y, m_p2z, p2, QString::fromLocal8Bit("P2"))) return;
    if (!parsePoint(m_q1x, m_q1y, m_q1z, q1, QString::fromLocal8Bit("Q1"))) return;
    if (!parsePoint(m_q2x, m_q2y, m_q2z, q2, QString::fromLocal8Bit("Q2"))) return;

    drawSegment(p1, p2, Quantity_NOC_BLUE1);
    drawSegment(q1, q2, Quantity_NOC_GREEN1);
    drawPoint(p1, Quantity_NOC_BLUE1, "P1");
    drawPoint(p2, Quantity_NOC_BLUE1, "P2");
    drawPoint(q1, Quantity_NOC_GREEN1, "Q1");
    drawPoint(q2, Quantity_NOC_GREEN1, "Q2");

    // Determine which method to use
    int methodValue = m_comboMethod->currentData().toInt();
    Method method = static_cast<Method>(methodValue);

    // Compute nearest points
    gp_Pnt near1, near2;
    switch (method) {
    case METHOD_ExtremaCurve:
        m_opUti->DistanceUsingExtremaCurveCurve(p1, p2, q1, q2, near1, near2);
        break;
    case METHOD_DistShapeShape:
        m_opUti->DistanceUsingDistShapeShape(p1, p2, q1, q2, near1, near2);
        break;
    case METHOD_ProjectPointOnCurve:
        m_opUti->DistanceUsingProjectPointOnCurve(p1, p2, q1, q2, near1, near2);
        break;
    case METHOD_ShapeAnalysisProject:
        m_opUti->DistanceUsingShapeAnalysisProject(p1, p2, q1, q2, near1, near2);
        break;
    case METHOD_Manual:
        m_opUti->DistanceUsingManual(p1, p2, q1, q2, near1, near2);
        break;
    default:
        // Unknown method; do nothing
        return;
    }

    double distance = near1.Distance(near2);
    drawPoint(near1, Quantity_NOC_RED, QString::fromLocal8Bit("近点1"));
    drawPoint(near2, Quantity_NOC_RED, QString::fromLocal8Bit("近点2"));
    if(near1.Distance(near2)>1)
        drawSegment(near1, near2, Quantity_NOC_RED);

    // Prepare the result text with nearest point information
    QString methodName = m_comboMethod->currentText();
    // Format the distance and the coordinates of near points. Adjust the precision as needed.
    QString resultText = QString::fromLocal8Bit("%1 最近距离: %2，近点1:(%3,%4,%5)，近点2:(%6,%7,%8)")
        .arg(methodName)
        .arg(distance, 0, 'f', 6)
        .arg(near1.X(), 0, 'f', 3)
        .arg(near1.Y(), 0, 'f', 3)
        .arg(near1.Z(), 0, 'f', 3)
        .arg(near2.X(), 0, 'f', 3)
        .arg(near2.Y(), 0, 'f', 3)
        .arg(near2.Z(), 0, 'f', 3);
    // Display the result in the label and output callback
    m_labelResult->setText(resultText);
    if (m_outputFunc) {
        m_outputFunc(resultText);
    }
}