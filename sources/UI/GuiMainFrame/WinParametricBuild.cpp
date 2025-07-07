#include "WinParametricBuild.h"






WinParametricBuild::WinParametricBuild(Handle(AIS_InteractiveContext) context,Handle(V3d_Viewer) v3dViewer,Handle(V3d_View) v3dView,QWidget* parent, std::function<void(const QString&)> func): QDialog(parent),
    m_context(context), m_v3dViewer(v3dViewer), m_v3dView(v3dView)
{
    setupUI();
    m_outputFunc = std::move(func);
}


void WinParametricBuild::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 线段
    QGroupBox* lineGroup = new QGroupBox(QString::fromLocal8Bit("线段"));
    QFormLayout* lineForm = new QFormLayout;
    m_lineStartX = new QLineEdit; m_lineStartY = new QLineEdit; m_lineStartZ = new QLineEdit;
    m_lineEndX = new QLineEdit;   m_lineEndY = new QLineEdit;   m_lineEndZ = new QLineEdit;
    QWidget* lineStartWidget = new QWidget;
    QHBoxLayout* hLineStart = new QHBoxLayout(lineStartWidget);
    hLineStart->addWidget(m_lineStartX); hLineStart->addWidget(m_lineStartY); hLineStart->addWidget(m_lineStartZ);
    QWidget* lineEndWidget = new QWidget;
    QHBoxLayout* hLineEnd = new QHBoxLayout(lineEndWidget);
    hLineEnd->addWidget(m_lineEndX); hLineEnd->addWidget(m_lineEndY); hLineEnd->addWidget(m_lineEndZ);
    m_btnDrawLine = new QPushButton(QString::fromLocal8Bit("绘制线段"));
    lineForm->addRow(QString::fromLocal8Bit("起点(x,y,z)"), lineStartWidget);
    lineForm->addRow(QString::fromLocal8Bit("终点(x,y,z)"), lineEndWidget);
    lineForm->addRow("", m_btnDrawLine);
    lineGroup->setLayout(lineForm);

    // 圆
    QGroupBox* circleGroup = new QGroupBox(QString::fromLocal8Bit("圆"));
    QFormLayout* circleForm = new QFormLayout;
    m_circleX = new QLineEdit; m_circleY = new QLineEdit; m_circleZ = new QLineEdit; m_circleRadius = new QLineEdit;
    QWidget* circleCenterWidget = new QWidget;
    QHBoxLayout* hCircleCenter = new QHBoxLayout(circleCenterWidget);
    hCircleCenter->addWidget(m_circleX); hCircleCenter->addWidget(m_circleY); hCircleCenter->addWidget(m_circleZ);
    m_btnDrawCircle = new QPushButton(QString::fromLocal8Bit("绘制圆"));
    circleForm->addRow(QString::fromLocal8Bit("圆心(x,y,z)"), circleCenterWidget);
    circleForm->addRow(QString::fromLocal8Bit("半径"), m_circleRadius);
    circleForm->addRow("", m_btnDrawCircle);
    circleGroup->setLayout(circleForm);

    // 球
    QGroupBox* sphereGroup = new QGroupBox(QString::fromLocal8Bit("球"));
    QFormLayout* sphereForm = new QFormLayout;
    m_sphereX = new QLineEdit; m_sphereY = new QLineEdit; m_sphereZ = new QLineEdit; m_sphereRadius = new QLineEdit;
    QWidget* sphereCenterWidget = new QWidget;
    QHBoxLayout* hSphereCenter = new QHBoxLayout(sphereCenterWidget);
    hSphereCenter->addWidget(m_sphereX); hSphereCenter->addWidget(m_sphereY); hSphereCenter->addWidget(m_sphereZ);
    m_btnDrawSphere = new QPushButton(QString::fromLocal8Bit("绘制球"));
    sphereForm->addRow(QString::fromLocal8Bit("球心(x,y,z)"), sphereCenterWidget);
    sphereForm->addRow(QString::fromLocal8Bit("半径"), m_sphereRadius);
    sphereForm->addRow("", m_btnDrawSphere);
    sphereGroup->setLayout(sphereForm);

    // Box
    QGroupBox* boxGroup = new QGroupBox(QString::fromLocal8Bit("长方体(Box)"));
    QFormLayout* boxForm = new QFormLayout;
    m_boxMinX = new QLineEdit; m_boxMinY = new QLineEdit; m_boxMinZ = new QLineEdit;
    m_boxMaxX = new QLineEdit; m_boxMaxY = new QLineEdit; m_boxMaxZ = new QLineEdit;
    QWidget* boxMinWidget = new QWidget;
    QHBoxLayout* hBoxMin = new QHBoxLayout(boxMinWidget);
    hBoxMin->addWidget(m_boxMinX); hBoxMin->addWidget(m_boxMinY); hBoxMin->addWidget(m_boxMinZ);
    QWidget* boxMaxWidget = new QWidget;
    QHBoxLayout* hBoxMax = new QHBoxLayout(boxMaxWidget);
    hBoxMax->addWidget(m_boxMaxX); hBoxMax->addWidget(m_boxMaxY); hBoxMax->addWidget(m_boxMaxZ);
    m_btnDrawBox = new QPushButton(QString::fromLocal8Bit("绘制Box"));
    boxForm->addRow(QString::fromLocal8Bit("最小点(x,y,z)"), boxMinWidget);
    boxForm->addRow(QString::fromLocal8Bit("最大点(x,y,z)"), boxMaxWidget);
    boxForm->addRow("", m_btnDrawBox);
    boxGroup->setLayout(boxForm);

    // 清空
    m_btnClear = new QPushButton(QString::fromLocal8Bit("清空场景"));

    mainLayout->addWidget(lineGroup);
    mainLayout->addWidget(circleGroup);
    mainLayout->addWidget(sphereGroup);
    mainLayout->addWidget(boxGroup);
    mainLayout->addWidget(m_btnClear);

    setLayout(mainLayout);

    // 信号槽
    connect(m_btnDrawLine, &QPushButton::clicked, this, &WinParametricBuild::onDrawLine);
    connect(m_btnDrawCircle, &QPushButton::clicked, this, &WinParametricBuild::onDrawCircle);
    connect(m_btnDrawSphere, &QPushButton::clicked, this, &WinParametricBuild::onDrawSphere);
    connect(m_btnDrawBox, &QPushButton::clicked, this, &WinParametricBuild::onDrawBox);
    connect(m_btnClear, &QPushButton::clicked, this, &WinParametricBuild::onClear);
}

// ----------- 槽函数 -------------------
void WinParametricBuild::onDrawLine()
{
    gp_Pnt p1, p2;
    if (!parsePoint(m_lineStartX, m_lineStartY, m_lineStartZ, p1, QString::fromLocal8Bit("起点")) ||
        !parsePoint(m_lineEndX, m_lineEndY, m_lineEndZ, p2, QString::fromLocal8Bit("终点"))) return;
    drawLine(p1, p2);
}
void WinParametricBuild::onDrawCircle()
{
    gp_Pnt center; double r;
    if (!parsePoint(m_circleX, m_circleY, m_circleZ, center, QString::fromLocal8Bit("圆心") )||
        !parseDouble(m_circleRadius, r, QString::fromLocal8Bit("半径"))) return;
    drawCircle(center, r);
}
void WinParametricBuild::onDrawSphere()
{
    gp_Pnt center; double r;
    if (!parsePoint(m_sphereX, m_sphereY, m_sphereZ, center, QString::fromLocal8Bit("球心")) ||
        !parseDouble(m_sphereRadius, r, QString::fromLocal8Bit( "半径"))) return;
    drawSphere(center, r);
}
void WinParametricBuild::onDrawBox()
{
    gp_Pnt minPt, maxPt;
    if (!parsePoint(m_boxMinX, m_boxMinY, m_boxMinZ, minPt, QString::fromLocal8Bit("最小点")) ||
        !parsePoint(m_boxMaxX, m_boxMaxY, m_boxMaxZ, maxPt, QString::fromLocal8Bit("最大点"))) return;
    if (maxPt.X() <= minPt.X() || maxPt.Y() <= minPt.Y() || maxPt.Z() <= minPt.Z()) {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("最大点必须大于最小点！"));
        return;
    }
    drawBox(minPt, maxPt);
}
void WinParametricBuild::onClear()
{
        // 获取所有正在显示的交互对象
        AIS_ListOfInteractive list;
        m_context->DisplayedObjects(list);
    
        std::vector<Handle(AIS_InteractiveObject)> toRemove;
        for (AIS_ListOfInteractive::Iterator it(list); it.More(); it.Next())
        {
            Handle(AIS_InteractiveObject) obj = it.Value();
            if (!obj.IsNull() && obj->IsKind(STANDARD_TYPE(AIS_Shape)))
            {
                toRemove.push_back(obj);
            }
        }
    
        for (const auto& obj : toRemove)
            m_context->Remove(obj, Standard_False);
    
        m_context->UpdateCurrentViewer();
    m_context->UpdateCurrentViewer();
}



// ----------- 绘制实现 ------------------
void WinParametricBuild::drawLine(const gp_Pnt& p1, const gp_Pnt& p2)
{
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);
    Handle(AIS_Shape) aisShape = new AIS_Shape(edge);
    aisShape->SetColor(Quantity_NOC_RED);
    aisShape->SetWidth(2.0);
    m_context->Display(aisShape, Standard_True);
    if (m_outputFunc)
    {
        m_outputFunc(
            QStringLiteral("绘制线段：\n  起点 ( %1, %2, %3 )\n  终点 ( %4, %5, %6 )")
            .arg(p1.X(), 0, 'f', 3)
            .arg(p1.Y(), 0, 'f', 3)
            .arg(p1.Z(), 0, 'f', 3)
            .arg(p2.X(), 0, 'f', 3)
            .arg(p2.Y(), 0, 'f', 3)
            .arg(p2.Z(), 0, 'f', 3)
        );
    }
}

void WinParametricBuild::drawCircle(const gp_Pnt& center, double radius)
{
    gp_Ax2 axis(center, gp_Dir(0, 0, 1));
    Handle(Geom_Circle) circle = new Geom_Circle(axis, radius);
    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circle);
    Handle(AIS_Shape) aisShape = new AIS_Shape(edge);
    aisShape->SetColor(Quantity_NOC_BLUE1);
    aisShape->SetWidth(2.0);
    m_context->Display(aisShape, Standard_True);

    if (m_outputFunc)
    {
        m_outputFunc(
            QStringLiteral("绘制圆：\n  圆心 ( %1, %2, %3 )\n  半径 %4")
            .arg(center.X(), 0, 'f', 3)
            .arg(center.Y(), 0, 'f', 3)
            .arg(center.Z(), 0, 'f', 3)
            .arg(radius, 0, 'f', 3)
        );
    }
}

void WinParametricBuild::drawSphere(const gp_Pnt& center, double radius)
{
    TopoDS_Shape sphere = BRepPrimAPI_MakeSphere(center, radius).Shape();
    Handle(AIS_Shape) aisShape = new AIS_Shape(sphere);
    aisShape->SetColor(Quantity_NOC_GREEN3);
    m_context->Display(aisShape, Standard_True);
    if (m_outputFunc)
    {
        m_outputFunc(
            QStringLiteral("绘制球体：\n  球心 ( %1, %2, %3 )\n  半径 %4")
            .arg(center.X(), 0, 'f', 3)
            .arg(center.Y(), 0, 'f', 3)
            .arg(center.Z(), 0, 'f', 3)
            .arg(radius, 0, 'f', 3)
        );
    }
}

void WinParametricBuild::drawBox(const gp_Pnt& minPt, const gp_Pnt& maxPt)
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(minPt, maxPt).Shape();
    Handle(AIS_Shape) aisShape = new AIS_Shape(box);
    aisShape->SetColor(Quantity_NOC_GREEN1);
    m_context->Display(aisShape, Standard_True);
    if (m_outputFunc)
    {
        m_outputFunc(
            QStringLiteral("绘制立方体：\n  最小点 ( %1, %2, %3 )\n  最大点 ( %4, %5, %6 )")
            .arg(minPt.X(), 0, 'f', 3)
            .arg(minPt.Y(), 0, 'f', 3)
            .arg(minPt.Z(), 0, 'f', 3)
            .arg(maxPt.X(), 0, 'f', 3)
            .arg(maxPt.Y(), 0, 'f', 3)
            .arg(maxPt.Z(), 0, 'f', 3)
        );
    }
}




// ----------- 工具函数 ------------------
bool WinParametricBuild::parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip)
{
    bool ok1, ok2, ok3;
    double x = xEdit->text().toDouble(&ok1);
    double y = yEdit->text().toDouble(&ok2);
    double z = zEdit->text().toDouble(&ok3);
    if (!ok1 || !ok2 || !ok3) {
        QMessageBox::warning(this, "输入错误", tip + "必须是数字");
        return false;
    }
    pt.SetCoord(x, y, z);
    return true;
}
bool WinParametricBuild::parseDouble(QLineEdit* edit, double& value, const QString& tip)
{
    bool ok;
    value = edit->text().toDouble(&ok);
    if (!ok || value <= 0) {
        QMessageBox::warning(this, "输入错误", tip + "必须为正数");
        return false;
    }
    return true;
}
