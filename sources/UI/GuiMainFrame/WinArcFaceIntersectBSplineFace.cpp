#include "WinArcFaceIntersectBSplineFace.h"
#include "JNew.h"
#include "GeometryUtilityOccDefine.h"




WinArcFaceIntersectBSplineFace::WinArcFaceIntersectBSplineFace(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView, QWidget* parent, std::function<void(const QString&)> func)
    : QDialog(parent), m_context(context), m_v3dViewer(v3dViewer), m_v3dView(v3dView), m_outputFunc(func)
{
    setupUI();
    setWindowTitle(QString::fromLocal8Bit("圆弧曲面 × B样条面相交测试"));
    setMinimumSize(720, 600);

    m_util = NEW_AS_OWNER_PTR(FreeFaceIntersectFreeFace);
}



QWidget* WinArcFaceIntersectBSplineFace::makeXYZRow(const QString& title,
    QLineEdit*& ex, QLineEdit*& ey, QLineEdit*& ez,
    const QString& x0, const QString& y0, const QString& z0)
{
    ex = new QLineEdit(x0); ey = new QLineEdit(y0); ez = new QLineEdit(z0);
    QWidget* w = new QWidget; auto* h = new QHBoxLayout(w);
    h->addWidget(new QLabel(title + QString::fromLocal8Bit("X:"))); h->addWidget(ex);
    h->addWidget(new QLabel(QString::fromLocal8Bit("Y:"))); h->addWidget(ey);
    h->addWidget(new QLabel(QString::fromLocal8Bit("Z:"))); h->addWidget(ez);
    h->setContentsMargins(0, 0, 0, 0);
    return w;
}

QWidget* WinArcFaceIntersectBSplineFace::makeCtrlRow(const QString& name,
    QLineEdit*& ex, QLineEdit*& ey, QLineEdit*& ez,
    const gp_Pnt& P)
{
    ex = new QLineEdit(QString::number(P.X()));
    ey = new QLineEdit(QString::number(P.Y()));
    ez = new QLineEdit(QString::number(P.Z()));
    QWidget* w = new QWidget; auto* h = new QHBoxLayout(w);
    h->addWidget(new QLabel(QString::fromLocal8Bit("%1: ").arg(name)));
    h->addWidget(ex); h->addWidget(ey); h->addWidget(ez);
    h->setContentsMargins(0, 0, 0, 0);
    return w;
}

void WinArcFaceIntersectBSplineFace::setupUI()
{
    // 圆弧曲面输入
    QWidget* wCenter = makeXYZRow(QString::fromLocal8Bit("圆心"), m_arcCX, m_arcCY, m_arcCZ, "0", "0", "6");
    m_arcR = new QLineEdit("20");
    m_arcS = new QLineEdit("-170");
    m_arcE = new QLineEdit("170");
    m_arcExtrusion = new QLineEdit("10");

    // B样条面控制点（默认一个有起伏的面）
    QWidget* w11 = makeCtrlRow("P11", m_s11x, m_s11y, m_s11z, gp_Pnt(-20, -20, 6));
    QWidget* w12 = makeCtrlRow("P12", m_s12x, m_s12y, m_s12z, gp_Pnt(0, -20, 2));
    QWidget* w13 = makeCtrlRow("P13", m_s13x, m_s13y, m_s13z, gp_Pnt(20, -20, 6));
    QWidget* w21 = makeCtrlRow("P21", m_s21x, m_s21y, m_s21z, gp_Pnt(-20, 0, 10));
    QWidget* w22 = makeCtrlRow("P22", m_s22x, m_s22y, m_s22z, gp_Pnt(0, 0, 6));
    QWidget* w23 = makeCtrlRow("P23", m_s23x, m_s23y, m_s23z, gp_Pnt(20, 0, 10));
    QWidget* w31 = makeCtrlRow("P31", m_s31x, m_s31y, m_s31z, gp_Pnt(-20, 20, 6));
    QWidget* w32 = makeCtrlRow("P32", m_s32x, m_s32y, m_s32z, gp_Pnt(0, 20, 2));
    QWidget* w33 = makeCtrlRow("P33", m_s33x, m_s33y, m_s33z, gp_Pnt(20, 20, 6));

    // 参数/方法
    m_comboMethod = new QComboBox;
    m_comboMethod->addItems({
        QString::fromLocal8Bit("Section (拓扑截取法)"),
        QString::fromLocal8Bit("IntSS (几何交线法)"),
        });

    m_spinTol3d = new QDoubleSpinBox;  m_spinTol3d->setDecimals(9);  m_spinTol3d->setRange(1e-12, 1e-1); m_spinTol3d->setValue(1e-7);
    m_spinTolNear = new QDoubleSpinBox; m_spinTolNear->setDecimals(9); m_spinTolNear->setRange(1e-12, 1e-1); m_spinTolNear->setValue(1e-5);
    m_spinSamples = new QSpinBox;       m_spinSamples->setRange(2, 2000); m_spinSamples->setValue(200);
    m_chkRefine = new QCheckBox(QString::fromLocal8Bit("投影精修 (RefineOnSurface)")); m_chkRefine->setChecked(true);

    m_btnCompute = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClear = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnCompute, &QPushButton::clicked, this, &WinArcFaceIntersectBSplineFace::onCompute);
    connect(m_btnClear, &QPushButton::clicked, this, &WinArcFaceIntersectBSplineFace::onClear);

    m_labelInfo = new QLabel;

    // 表单布局
    auto* form = new QFormLayout;
    form->addRow(QString::fromLocal8Bit("圆心："), wCenter);
    QWidget* wRE = new QWidget; auto* hRE = new QHBoxLayout(wRE);
    hRE->addWidget(new QLabel(QString::fromLocal8Bit("半径")));       hRE->addWidget(m_arcR);
    hRE->addWidget(new QLabel(QString::fromLocal8Bit("起始角(°)"))); hRE->addWidget(m_arcS);
    hRE->addWidget(new QLabel(QString::fromLocal8Bit("终止角(°)"))); hRE->addWidget(m_arcE);
    hRE->addWidget(new QLabel(QString::fromLocal8Bit("拉伸距离")));  hRE->addWidget(m_arcExtrusion);
    hRE->setContentsMargins(0, 0, 0, 0);
    form->addRow(QString(), wRE);

    form->addRow("P11", w11); form->addRow("P12", w12); form->addRow("P13", w13);
    form->addRow("P21", w21); form->addRow("P22", w22); form->addRow("P23", w23);
    form->addRow("P31", w31); form->addRow("P32", w32); form->addRow("P33", w33);

    auto* grid = new QGridLayout;
    grid->addWidget(new QLabel(QString::fromLocal8Bit("方法：")), 0, 0); grid->addWidget(m_comboMethod, 0, 1);
    grid->addWidget(new QLabel("tol3d:"), 1, 0);   grid->addWidget(m_spinTol3d, 1, 1);
    grid->addWidget(new QLabel("tolNear:"), 2, 0); grid->addWidget(m_spinTolNear, 2, 1);
    grid->addWidget(new QLabel(QString::fromLocal8Bit("采样数 (Discretize)：")), 3, 0); grid->addWidget(m_spinSamples, 3, 1);
    grid->addWidget(m_chkRefine, 4, 0, 1, 2);

    auto* btns = new QHBoxLayout;
    btns->addWidget(m_btnCompute);
    btns->addWidget(m_btnClear);
    btns->setContentsMargins(0, 0, 0, 0);

    auto* main = new QVBoxLayout(this);
    main->addLayout(form);
    main->addLayout(grid);
    main->addLayout(btns);
    main->addWidget(m_labelInfo);
}




void WinArcFaceIntersectBSplineFace::drawArcFace(const gp_Pnt& center, double radius, double startAngle, double endAngle, double extrusion)
{
    // 创建圆弧边
    Handle(Geom_Circle) circ = new Geom_Circle(gp_Ax2(center, gp::DZ()), radius);
    Handle(Geom_TrimmedCurve) arc = new Geom_TrimmedCurve(circ, startAngle, endAngle);
    TopoDS_Edge arcEdge = BRepBuilderAPI_MakeEdge(arc);

    // 创建线
    BRepBuilderAPI_MakeWire wireMaker(arcEdge);
    if (!wireMaker.IsDone()) 
    {
        // 处理错误
        return;
    }
    TopoDS_Wire wire = wireMaker.Wire();

    // 拉伸成面 - 使用 BRepPrimAPI_MakePrism 而不是 BRepBuilderAPI_MakePrism
    gp_Vec extrusionVec(0, 0, extrusion);
    BRepPrimAPI_MakePrism prismMaker(wire, extrusionVec);
    if (!prismMaker.IsDone()) {
        // 处理错误
        return;
    }

    // 从棱柱体中提取面
    TopoDS_Shape prismShape = prismMaker.Shape();

    // 提取面（棱柱体可能有多个面，我们取第一个）
    TopExp_Explorer faceExplorer(prismShape, TopAbs_FACE);
    if (faceExplorer.More()) {
        TopoDS_Face arcFace = TopoDS::Face(faceExplorer.Current());
        Handle(AIS_Shape) shape = new AIS_Shape(arcFace);
        m_context->Display(shape, Standard_False);
        m_myObjects.append(shape);
    }
}

void WinArcFaceIntersectBSplineFace::drawBSplineFace(const Handle(Geom_BSplineSurface)& surf)
{
    TopoDS_Face face = BRepBuilderAPI_MakeFace(surf, Precision::Confusion());
    Handle(AIS_Shape) shape = new AIS_Shape(face);
    m_context->Display(shape, Standard_False);
    m_myObjects.append(shape);
}

void WinArcFaceIntersectBSplineFace::drawEdges(const std::vector<TopoDS_Edge>& edges, Quantity_NameOfColor color)
{
    for (const auto& edge : edges)
    {
        Handle(AIS_Shape) shape = new AIS_Shape(edge);
        shape->SetColor(color);
        m_context->Display(shape, Standard_False);
        m_myObjects.append(shape);
    }
}

void WinArcFaceIntersectBSplineFace::onClear()
{
    for (const auto& obj : m_myObjects)
        m_context->Remove(obj, Standard_False);
    m_myObjects.clear();
    m_context->UpdateCurrentViewer();
    m_labelInfo->clear();
}




void WinArcFaceIntersectBSplineFace::onCompute()
{
    onClear();

    // 读取圆弧曲面输入
    bool okR, okS, okE, okx, oky, okz, okExt;
    double R = m_arcR->text().toDouble(&okR);
    double Sd = m_arcS->text().toDouble(&okS);
    double Ed = m_arcE->text().toDouble(&okE);
    double cx = m_arcCX->text().toDouble(&okx);
    double cy = m_arcCY->text().toDouble(&oky);
    double cz = m_arcCZ->text().toDouble(&okz);
    double extrusion = m_arcExtrusion->text().toDouble(&okExt);

    if (!(okR && okS && okE && okx && oky && okz && okExt) || R <= 0 || extrusion <= 0) {
        QMessageBox::warning(this, QString::fromLocal8Bit("输入错误"), QString::fromLocal8Bit("圆弧曲面参数非法"));
        return;
    }

    double S = Sd * M_PI / 180.0;
    double E = Ed * M_PI / 180.0;
    gp_Pnt center(cx, cy, cz);
    // 构造圆弧曲面
    Handle(Geom_Circle) circ = new Geom_Circle(gp_Ax2(center, gp::DZ()), R);
    Handle(Geom_TrimmedCurve) arc = new Geom_TrimmedCurve(circ, S, E);
    TopoDS_Edge arcEdge = BRepBuilderAPI_MakeEdge(arc);

    // 创建线
    BRepBuilderAPI_MakeWire wireMaker(arcEdge);
    if (!wireMaker.IsDone()) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("无法创建线"));
        return;
    }
    TopoDS_Wire wire = wireMaker.Wire();

    // 拉伸成面 - 使用 BRepPrimAPI_MakePrism
    gp_Vec extrusionVec(0, 0, extrusion);
    BRepPrimAPI_MakePrism prismMaker(wire, extrusionVec);
    if (!prismMaker.IsDone()) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("无法拉伸成面"));
        return;
    }

    // 提取面
    TopoDS_Shape prismShape = prismMaker.Shape();
    TopExp_Explorer faceExplorer(prismShape, TopAbs_FACE);
    if (!faceExplorer.More()) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("无法从拉伸形状中提取面"));
        return;
    }
    TopoDS_Face arcFace = TopoDS::Face(faceExplorer.Current());

    // 读取 3×3 控制点，构造 B 样条面
    auto P = [&](QLineEdit* ex, QLineEdit* ey, QLineEdit* ez)->gp_Pnt {
        bool okx, oky, okz;
        double x = ex->text().toDouble(&okx);
        double y = ey->text().toDouble(&oky);
        double z = ez->text().toDouble(&okz);
        return gp_Pnt(okx ? x : 0, oky ? y : 0, okz ? z : 0);
        };

    TColgp_Array2OfPnt poles(1, 3, 1, 3);
    poles.SetValue(1, 1, P(m_s11x, m_s11y, m_s11z)); poles.SetValue(1, 2, P(m_s12x, m_s12y, m_s12z)); poles.SetValue(1, 3, P(m_s13x, m_s13y, m_s13z));
    poles.SetValue(2, 1, P(m_s21x, m_s21y, m_s21z)); poles.SetValue(2, 2, P(m_s22x, m_s22y, m_s22z)); poles.SetValue(2, 3, P(m_s23x, m_s23y, m_s23z));
    poles.SetValue(3, 1, P(m_s31x, m_s31y, m_s31z)); poles.SetValue(3, 2, P(m_s32x, m_s32y, m_s32z)); poles.SetValue(3, 3, P(m_s33x, m_s33y, m_s33z));

    Handle(Geom_BSplineSurface) surf = GeomAPI_PointsToBSplineSurface(poles).Surface();
    TopoDS_Face bsplineFace = BRepBuilderAPI_MakeFace(surf, Precision::Confusion());

    // 显示几何
    drawArcFace(center, R, S, E, extrusion);
    drawBSplineFace(surf);

    // 参数与方法
    EnumFreeFaceIntersectFreeFaceMethod method;
    switch (m_comboMethod->currentIndex()) {
    case 0: method = EnumFreeFaceIntersectFreeFaceMethod::Section; break;
    case 1: method = EnumFreeFaceIntersectFreeFaceMethod::Int_SS; break;
    default: method = EnumFreeFaceIntersectFreeFaceMethod::Section;
    }

    // 求交
    std::vector<TopoDS_Edge> edges = m_util->IntersectFaceWithFace(arcFace, bsplineFace, method);

    // 绘制交线
    drawEdges(edges, Quantity_NOC_GREEN);

    // 信息
    QString info = QString::fromLocal8Bit("%1：%2 条交线边   tol3d=%3   tolNear=%4   samples=%5")
        .arg(m_comboMethod->currentText())
        .arg(edges.size())
        .arg(m_spinTol3d->value(), 0, 'g', 6)
        .arg(m_spinTolNear->value(), 0, 'g', 6)
        .arg(m_spinSamples->value());
    m_labelInfo->setText(info);

    m_context->UpdateCurrentViewer();
}