#include "WinSegmentPlaneIntersection.h"
#include "JNew.h"



WinSegmentPlaneIntersection::WinSegmentPlaneIntersection(Handle(AIS_InteractiveContext) ctx,
    Handle(V3d_Viewer) viewer,
    Handle(V3d_View)    view,
    QWidget* parent, std::function<void(const QString&)> outputFunc )
    : QDialog(parent), m_context(ctx), m_viewer(viewer), m_view(view)
{
    setupUI();
    setWindowTitle(QString::fromLocal8Bit("线段-平面求交"));
    setMinimumSize(640, 420);

    m_opUti = NEW_AS_OWNER_PTR(GeomLineSegmentRelationUtility);
}

void WinSegmentPlaneIntersection::setupUI()
{
    auto mkXYZ = [&](QLineEdit*& ex, QLineEdit*& ey, QLineEdit*& ez,
        const char* dx, const char* dy, const char* dz)->QWidget*
        {
            ex = new QLineEdit(dx); ey = new QLineEdit(dy); ez = new QLineEdit(dz);
            auto* w = new QWidget;
            auto* l = new QHBoxLayout(w);
            l->addWidget(ex); l->addWidget(ey); l->addWidget(ez);
            l->setContentsMargins(0, 0, 0, 0);
            return w;
        };

    // 线段参数
    QWidget* wP1 = mkXYZ(m_p1x, m_p1y, m_p1z, "0", "0", "100");
    QWidget* wP2 = mkXYZ(m_p2x, m_p2y, m_p2z, "300", "100", "200");

    // 平面参数（点+法向）与显示尺寸
    QWidget* wP0 = mkXYZ(m_p0x, m_p0y, m_p0z, "0", "0", "0");
    QWidget* wN = mkXYZ(m_nx, m_ny, m_nz, "0", "0", "1");
    m_halfSize = new QLineEdit("500");

    // 算法选择（与 switch-case 序号一一对应）
    m_combo = new QComboBox;
    m_combo->addItem(QString::fromLocal8Bit("距离法 (点到面)"));                  // 0
    m_combo->addItem(QString::fromLocal8Bit("解析法 (参数t)"));                  // 1  (注意你源码函数名写成 OmtersectSegementPlaneUsingAnalytic)
    m_combo->addItem(QString::fromLocal8Bit("GeomAPI_IntCS"));                  // 2
    m_combo->addItem(QString::fromLocal8Bit("BRepAlgoAPI_Section"));            // 3
    m_combo->addItem(QString::fromLocal8Bit("IntCurvesFace_ShapeIntersector")); // 4
    m_combo->addItem(QString::fromLocal8Bit("IntAna_IntConicQuad"));            // 5

    auto* groupMethod = new QGroupBox(QString::fromLocal8Bit("计算方式"));
    auto* layMethod = new QVBoxLayout(groupMethod);
    layMethod->addWidget(m_combo);
    layMethod->setContentsMargins(0, 0, 0, 0);

    // 按钮
    m_btnDraw = new QPushButton(QString::fromLocal8Bit("绘制"));
    m_btnCompute = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClear = new QPushButton(QString::fromLocal8Bit("清空"));

    connect(m_btnDraw, &QPushButton::clicked, this, &WinSegmentPlaneIntersection::onDraw);
    connect(m_btnCompute, &QPushButton::clicked, this, &WinSegmentPlaneIntersection::onCompute);
    connect(m_btnClear, &QPushButton::clicked, this, &WinSegmentPlaneIntersection::onClear);

    auto* layBtns = new QHBoxLayout;
    layBtns->addWidget(m_btnDraw);
    layBtns->addWidget(m_btnCompute);
    layBtns->addWidget(m_btnClear);
    layBtns->setContentsMargins(0, 0, 0, 0);

    // 结果
    m_result = new QLabel;

    // 表单
    auto* form = new QFormLayout;
    form->addRow(QString::fromLocal8Bit("线段起点 P1:"), wP1);
    form->addRow(QString::fromLocal8Bit("线段终点 P2:"), wP2);
    form->addRow(QString::fromLocal8Bit("平面点 P0:"), wP0);
    form->addRow(QString::fromLocal8Bit("平面法向 N:"), wN);
    form->addRow(QString::fromLocal8Bit("平面半尺寸:"), m_halfSize);

    auto* main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addWidget(groupMethod);
    main->addLayout(layBtns);
    main->addWidget(m_result);
}

bool WinSegmentPlaneIntersection::parsePoint(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez, gp_Pnt& P, const QString& tip)
{
    bool okx = false, oky = false, okz = false;
    double x = ex->text().toDouble(&okx);
    double y = ey->text().toDouble(&oky);
    double z = ez->text().toDouble(&okz);
    if (!okx || !oky || !okz)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), tip + QString::fromLocal8Bit("坐标无效"));
        return false;
    }
    P.SetCoord(x, y, z);
    return true;
}

bool WinSegmentPlaneIntersection::parseVector(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez, gp_Vec& V, const QString& tip)
{
    gp_Pnt tmp;
    if (!parsePoint(ex, ey, ez, tmp, tip)) return false;
    V = gp_Vec(tmp.X(), tmp.Y(), tmp.Z());
    return true;
}

bool WinSegmentPlaneIntersection::parseDouble(QLineEdit* e, double& v, const QString& tip)
{
    bool ok = false;
    v = e->text().toDouble(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), tip + QString::fromLocal8Bit("无效"));
        return false;
    }
    return true;
}

void WinSegmentPlaneIntersection::onClear()
{
    for (const auto& obj : m_objects)
        m_context->Remove(obj, Standard_False);
    m_objects.clear();
    m_result->clear();
    m_context->UpdateCurrentViewer();
}

void WinSegmentPlaneIntersection::drawSegment(const gp_Pnt& p1, const gp_Pnt& p2)
{
    Handle(AIS_Shape) ais = new AIS_Shape(BRepBuilderAPI_MakeEdge(p1, p2));
    m_context->Display(ais, Standard_True);
    m_objects.append(ais);
}

void WinSegmentPlaneIntersection::drawPlane(const gp_Pln& pln, double halfSize)
{
    TopoDS_Face face = BRepBuilderAPI_MakeFace(pln, -halfSize, +halfSize, -halfSize, +halfSize);
    Handle(AIS_Shape) ais = new AIS_Shape(face);
    ais->SetColor(Quantity_NOC_GRAY85);
    ais->SetTransparency(0.85);
    m_context->Display(ais, Standard_True);
    m_objects.append(ais);
}

void WinSegmentPlaneIntersection::drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& text)
{
    Handle(AIS_Point) pt = new AIS_Point(new Geom_CartesianPoint(p));
    pt->SetColor(color);
    m_context->Display(pt, Standard_True);
    m_objects.append(pt);

    Handle(AIS_TextLabel) label = new AIS_TextLabel();
    label->SetPosition(p.Translated(gp_Vec(2, 2, 2)));
    label->SetText(TCollection_ExtendedString(text.toStdWString().c_str()));
    label->SetColor(color);
    m_context->Display(label, Standard_False);
    m_objects.append(label);
}

void WinSegmentPlaneIntersection::drawDash(const gp_Pnt& a, const gp_Pnt& b, const Quantity_Color& col, Standard_Real width)
{
    Handle(AIS_Shape) line = new AIS_Shape(BRepBuilderAPI_MakeEdge(a, b));
    Handle(Prs3d_LineAspect) aspect = new Prs3d_LineAspect(col, Aspect_TOL_DASH, width);
    line->Attributes()->SetLineAspect(aspect);
    m_context->Display(line, Standard_False);
    m_objects.append(line);
}

void WinSegmentPlaneIntersection::onDraw()
{
    // 解析输入
    gp_Pnt P1, P2, P0;
    gp_Vec Nvec;
    double half = 500.0;

    if (!parsePoint(m_p1x, m_p1y, m_p1z, P1, QString::fromLocal8Bit("线段起点"))) return;
    if (!parsePoint(m_p2x, m_p2y, m_p2z, P2, QString::fromLocal8Bit("线段终点"))) return;
    if (!parsePoint(m_p0x, m_p0y, m_p0z, P0, QString::fromLocal8Bit("平面点 P0"))) return;
    if (!parseVector(m_nx, m_ny, m_nz, Nvec, QString::fromLocal8Bit("平面法向 N"))) return;
    if (!parseDouble(m_halfSize, half, QString::fromLocal8Bit("平面半尺寸"))) return;

    if (Nvec.SquareMagnitude() < Precision::SquareConfusion())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("法向量长度为 0"));
        return;
    }

    // 绘制
    onClear();
    drawSegment(P1, P2);
    gp_Pln pln(gp_Ax3(P0, gp_Dir(Nvec)));
    drawPlane(pln, std::max(1.0, half));
    m_context->UpdateCurrentViewer();
}

void WinSegmentPlaneIntersection::onCompute()
{
    // 解析输入（与绘制相同）
    gp_Pnt P1, P2, P0;
    gp_Vec Nvec;
    double half = 500.0;

    if (!parsePoint(m_p1x, m_p1y, m_p1z, P1, QString::fromLocal8Bit("线段起点"))) return;
    if (!parsePoint(m_p2x, m_p2y, m_p2z, P2, QString::fromLocal8Bit("线段终点"))) return;
    if (!parsePoint(m_p0x, m_p0y, m_p0z, P0, QString::fromLocal8Bit("平面点 P0"))) return;
    if (!parseVector(m_nx, m_ny, m_nz, Nvec, QString::fromLocal8Bit("平面法向 N"))) return;
    if (!parseDouble(m_halfSize, half, QString::fromLocal8Bit("平面半尺寸"))) return;

    if (Nvec.SquareMagnitude() < Precision::SquareConfusion())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("法向量长度为 0"));
        return;
    }

    // 先确保已绘制（不强制清空，便于对比）
    if (m_objects.isEmpty())
    {
        drawSegment(P1, P2);
        gp_Pln pln0(gp_Ax3(P0, gp_Dir(Nvec)));
        drawPlane(pln0, std::max(1.0, half));
    }

    gp_Pln plane(gp_Ax3(P0, gp_Dir(Nvec)));

    // 按选择算法计算
    const int method = m_combo->currentIndex();
    gp_Pnt ip;
    bool hit = false;

    switch (method)
    {
    case 0: // 距离法
        hit = m_opUti->IntersectSegmentPlaneUsingDistance(P1, P2, plane, ip);
        break;
    case 1: // 解析法（你源码里函数名是 OmtersectSegementPlaneUsingAnalytic）
        hit = m_opUti->OmtersectSegementPlaneUsingAnalytic(P1, P2, plane, ip);
        break;
    case 2: // GeomAPI_IntCS
        hit = m_opUti->IntersectSegmentPlaneUsingIntCS(P1, P2, plane, ip);
        break;
    case 3: // BRepAlgoAPI_Section
        hit = m_opUti->IntersectSegmentPlaneUsingSection(P1, P2, plane, ip);
        break;
    case 4: // IntCurvesFace_ShapeIntersector
        hit = m_opUti->IntersectSegmentPlaneUsingIntCurveFace(P1, P2, plane, ip);
        break;
    case 5: // IntAna_IntConicQuad
        hit = m_opUti->IntersectSegmentPlaneUsingIntAnaIntConicQuad(P1, P2, plane, ip);
        break;
    default:
        break;
    }

    // 展示结果
    if (hit)
    {
        drawPoint(ip, Quantity_NOC_GREEN, QString::fromLocal8Bit("交点"));
        // 辅助虚线：交点沿法向画一小段（可选）
        gp_Vec shortN = gp_Vec(gp_Dir(Nvec)) * (0.15 * std::max(1.0, half));
        drawDash(ip, ip.Translated(shortN), Quantity_Color(0, 0, 1, Quantity_TOC_RGB), 2.0);

        m_result->setText(
            QString::fromLocal8Bit("交点: (%1, %2, %3)")
            .arg(QString::number(ip.X(), 'f', 3))
            .arg(QString::number(ip.Y(), 'f', 3))
            .arg(QString::number(ip.Z(), 'f', 3))
        );
    }
    else
    {
        m_result->setText(QString::fromLocal8Bit("无交点：线段与平面平行或未穿过平面"));
    }

    m_context->UpdateCurrentViewer();
}
