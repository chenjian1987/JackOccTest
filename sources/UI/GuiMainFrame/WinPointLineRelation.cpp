#include <QLabel>
#include "AIS_TextLabel.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "Geom_Line.hxx"
#include "Geom_Circle.hxx"
#include "Geom_TrimmedCurve.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "Geom_BSplineCurve.hxx"
#include "gp_Lin.hxx"
#include "Geom_CartesianPoint.hxx"

#include "WinPointLineRelation.h"
#include "GuiMainOccDefines.h"


WinPointLineRelation::WinPointLineRelation(
    Handle(AIS_InteractiveContext) context,
    Handle(V3d_Viewer) v3dViewer,
    Handle(V3d_View) v3dView,
    QWidget* parent,
    std::function<void(const QString&)> func)
    : QDialog(parent),
    m_context(context),
    m_v3dViewer(v3dViewer),
    m_v3dView(v3dView),
    m_outputFunc(func)
{
    setupUI();
}

// --------------------- UI -----------------------
void WinPointLineRelation::setupUI()
{
    setWindowTitle(QString::fromLocal8Bit("点与曲线关系工具"));
    setMinimumSize(600, 400);

    QTabWidget* tabWidget = new QTabWidget(this);

    QWidget* tabLine = new QWidget;
    QWidget* tabArc = new QWidget;
    QWidget* tabBSpline = new QWidget;

    setupLineTab(tabLine);
    setupArcTab(tabArc);
    setupBSplineTab(tabBSpline);

    tabWidget->addTab(tabLine, QString::fromLocal8Bit("直线/线段"));
    tabWidget->addTab(tabArc, QString::fromLocal8Bit("圆弧"));
    tabWidget->addTab(tabBSpline, QString::fromLocal8Bit("B样条"));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void WinPointLineRelation::setupLineTab(QWidget* tab)
{
    // 起点
    m_lineStartX = new QLineEdit("0");
    m_lineStartY = new QLineEdit("0");
    m_lineStartZ = new QLineEdit("0");
    QWidget* widgetStart = new QWidget;
    QHBoxLayout* layoutStart = new QHBoxLayout(widgetStart);
    layoutStart->addWidget(m_lineStartX);
    layoutStart->addWidget(m_lineStartY);
    layoutStart->addWidget(m_lineStartZ);
    layoutStart->setContentsMargins(0, 0, 0, 0);

    // 终点
    m_lineEndX = new QLineEdit("0");
    m_lineEndY = new QLineEdit("0");
    m_lineEndZ = new QLineEdit("0");
    QWidget* widgetEnd = new QWidget;
    QHBoxLayout* layoutEnd = new QHBoxLayout(widgetEnd);
    layoutEnd->addWidget(m_lineEndX);
    layoutEnd->addWidget(m_lineEndY);
    layoutEnd->addWidget(m_lineEndZ);
    layoutEnd->setContentsMargins(0, 0, 0, 0);

    // 目标点
    m_pointX_Line = new QLineEdit("0");
    m_pointY_Line = new QLineEdit("0");
    m_pointZ_Line = new QLineEdit("0");
    QWidget* widgetTarget = new QWidget;
    QHBoxLayout* layoutTarget = new QHBoxLayout(widgetTarget);
    layoutTarget->addWidget(m_pointX_Line);
    layoutTarget->addWidget(m_pointY_Line);
    layoutTarget->addWidget(m_pointZ_Line);
    layoutTarget->setContentsMargins(0, 0, 0, 0);

    m_offsetDistance = new QLineEdit("0");

    m_radioLine = new QRadioButton(QString::fromLocal8Bit("直线"));
    m_radioSegment = new QRadioButton(QString::fromLocal8Bit("线段"));
    m_radioLine->setChecked(true);
    QGroupBox* groupLineType = new QGroupBox(QString::fromLocal8Bit("线类型"));
    QHBoxLayout* layoutLineType = new QHBoxLayout(groupLineType);
    layoutLineType->addWidget(m_radioLine);
    layoutLineType->addWidget(m_radioSegment);
    layoutLineType->setContentsMargins(0, 0, 0, 0);

    m_radioNearest = new QRadioButton(QString::fromLocal8Bit("最近点"));
  /*  m_radioSymmetric = new QRadioButton(QString::fromLocal8Bit("对称点"));
    m_radioOffset = new QRadioButton(QString::fromLocal8Bit("偏移点"));*/
    m_radioNearest->setChecked(true);
    QGroupBox* groupFunc = new QGroupBox(QString::fromLocal8Bit("功能"));
    QVBoxLayout* layoutFunc = new QVBoxLayout(groupFunc);
    layoutFunc->addWidget(m_radioNearest);
    //layoutFunc->addWidget(m_radioSymmetric);
    //layoutFunc->addWidget(m_radioOffset);
    layoutFunc->setContentsMargins(0, 0, 0, 0);

    m_btnComputeLine = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClearLine = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnComputeLine, &QPushButton::clicked, this, &WinPointLineRelation::onComputeLine);
    connect(m_btnClearLine, &QPushButton::clicked, this, &WinPointLineRelation::onClear);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeLine);
    btnLayout->addWidget(m_btnClearLine);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    m_labelResultLine = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("起点:"), widgetStart);
    formLayout->addRow(QString::fromLocal8Bit("终点:"), widgetEnd);
    formLayout->addRow(QString::fromLocal8Bit("目标点:"), widgetTarget);
    formLayout->addRow(QString::fromLocal8Bit("偏移距离:"), m_offsetDistance);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(groupLineType);
    mainLayout->addWidget(groupFunc);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultLine);
}

void WinPointLineRelation::setupArcTab(QWidget* tab)
{
    m_arcCenterX = new QLineEdit("0");
    m_arcCenterY = new QLineEdit("0");
    m_arcCenterZ = new QLineEdit("0");
    QWidget* widgetCenter = new QWidget;
    QHBoxLayout* layoutCenter = new QHBoxLayout(widgetCenter);
    layoutCenter->addWidget(m_arcCenterX);
    layoutCenter->addWidget(m_arcCenterY);
    layoutCenter->addWidget(m_arcCenterZ);
    layoutCenter->setContentsMargins(0, 0, 0, 0);

    m_arcRadius = new QLineEdit("50");
    m_arcStartAngle = new QLineEdit("0");
    m_arcEndAngle = new QLineEdit("90");

    m_pointX_Arc = new QLineEdit("0");
    m_pointY_Arc = new QLineEdit("0");
    m_pointZ_Arc = new QLineEdit("0");
    QWidget* widgetTarget = new QWidget;
    QHBoxLayout* layoutTarget = new QHBoxLayout(widgetTarget);
    layoutTarget->addWidget(m_pointX_Arc);
    layoutTarget->addWidget(m_pointY_Arc);
    layoutTarget->addWidget(m_pointZ_Arc);
    layoutTarget->setContentsMargins(0, 0, 0, 0);

    m_btnComputeArc = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClearArc = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnComputeArc, &QPushButton::clicked, this, &WinPointLineRelation::onComputeArc);
    connect(m_btnClearArc, &QPushButton::clicked, this, &WinPointLineRelation::onClear);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeArc);
    btnLayout->addWidget(m_btnClearArc);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    m_labelResultArc = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("圆心:"), widgetCenter);
    formLayout->addRow(QString::fromLocal8Bit("半径:"), m_arcRadius);
    formLayout->addRow(QString::fromLocal8Bit("起始角(度):"), m_arcStartAngle);
    formLayout->addRow(QString::fromLocal8Bit("终止角(度):"), m_arcEndAngle);
    formLayout->addRow(QString::fromLocal8Bit("目标点:"), widgetTarget);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultArc);
}

void WinPointLineRelation::setupBSplineTab(QWidget* tab)
{
    auto createPointWidget = [](QLineEdit*& x, QLineEdit*& y, QLineEdit*& z, const QString& xval, const QString& yval, const QString& zval)
        {
            x = new QLineEdit(xval);
            y = new QLineEdit(yval);
            z = new QLineEdit(zval);
            QWidget* w = new QWidget;
            QHBoxLayout* l = new QHBoxLayout(w);
            l->addWidget(x); l->addWidget(y); l->addWidget(z);
            l->setContentsMargins(0, 0, 0, 0);
            return w;
        };

    QWidget* wP1 = createPointWidget(m_bsplineP1X, m_bsplineP1Y, m_bsplineP1Z, "0", "0", "0");
    QWidget* wP2 = createPointWidget(m_bsplineP2X, m_bsplineP2Y, m_bsplineP2Z, "50", "0", "0");
    QWidget* wP3 = createPointWidget(m_bsplineP3X, m_bsplineP3Y, m_bsplineP3Z, "50", "50", "0");
    QWidget* wP4 = createPointWidget(m_bsplineP4X, m_bsplineP4Y, m_bsplineP4Z, "0", "50", "0");
    QWidget* wTarget = createPointWidget(m_pointX_BSpline, m_pointY_BSpline, m_pointZ_BSpline, "0", "0", "0");

    m_btnComputeBSpline = new QPushButton(QString::fromLocal8Bit("计算"));
    m_btnClearBSpline = new QPushButton(QString::fromLocal8Bit("清空"));
    connect(m_btnComputeBSpline, &QPushButton::clicked, this, &WinPointLineRelation::onComputeBSpline);
    connect(m_btnClearBSpline, &QPushButton::clicked, this, &WinPointLineRelation::onClear);

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_btnComputeBSpline);
    btnLayout->addWidget(m_btnClearBSpline);
    btnLayout->setContentsMargins(0, 0, 0, 0);

    m_labelResultBSpline = new QLabel;

    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(QString::fromLocal8Bit("控制点1:"), wP1);
    formLayout->addRow(QString::fromLocal8Bit("控制点2:"), wP2);
    formLayout->addRow(QString::fromLocal8Bit("控制点3:"), wP3);
    formLayout->addRow(QString::fromLocal8Bit("控制点4:"), wP4);
    formLayout->addRow(QString::fromLocal8Bit("目标点:"), wTarget);

    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_labelResultBSpline);
}



// 绘制
void WinPointLineRelation::drawLine(const gp_Pnt& p1, const gp_Pnt& p2, bool asSegment)
{
    onClear();
    Handle(AIS_Shape) shape;
    if (asSegment)
        shape = new AIS_Shape(BRepBuilderAPI_MakeEdge(p1, p2));
    else
    {
        gp_Dir dir(gp_Vec(p1, p2));
        Handle(Geom_Line) line = new Geom_Line(p1, dir);
        shape = new AIS_Shape(BRepBuilderAPI_MakeEdge(line, -1000, +1000));
    }
    m_context->Display(shape, Standard_True);
    m_myObjects.append(shape);
}

void WinPointLineRelation::drawArc(const gp_Pnt& center, double radius, double startAngle, double endAngle)
{
    onClear();
    gp_Ax2 plane(center, gp_Dir(0, 0, 1));
    Handle(Geom_Circle) circle = new Geom_Circle(plane, radius);
    Handle(Geom_TrimmedCurve) arc = new Geom_TrimmedCurve(circle, startAngle, endAngle);
    Handle(AIS_Shape) shape = new AIS_Shape(BRepBuilderAPI_MakeEdge(arc));
    m_context->Display(shape, Standard_True);
    m_myObjects.append(shape);
}

void WinPointLineRelation::drawBSpline(const std::vector<gp_Pnt>& controlPoints)
{
    onClear();
    TColgp_Array1OfPnt poles(1, int(controlPoints.size()));
    for (Standard_Integer i = 1; i <= controlPoints.size(); ++i)
        poles(i) = controlPoints[i - 1];
    Handle(Geom_BSplineCurve) bspline = GeomAPI_PointsToBSpline(poles);
    Handle(AIS_Shape) shape = new AIS_Shape(BRepBuilderAPI_MakeEdge(bspline));
    m_context->Display(shape, Standard_True);
    m_myObjects.append(shape);
}

void WinPointLineRelation::drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name)
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

void WinPointLineRelation::drawPerpendicular(const gp_Pnt& target, const gp_Pnt& nearest)
{
    Handle(AIS_Shape) line = new AIS_Shape(BRepBuilderAPI_MakeEdge(target, nearest));
    Quantity_Color col(0, 0, 1, Quantity_TOC_RGB);
    Handle(Prs3d_LineAspect) aspect = new Prs3d_LineAspect(col, Aspect_TOL_DASH, 2.0);
    line->SetColor(col);
    line->Attributes()->SetLineAspect(aspect);
    m_context->Display(line, Standard_False);
    m_myObjects.append(line);
}

void WinPointLineRelation::drawExtension(const gp_Pnt& endpoint, const gp_Pnt& nearest)
{
    if (endpoint.Distance(nearest) < 1) return;
    Handle(AIS_Shape) line = new AIS_Shape(BRepBuilderAPI_MakeEdge(endpoint, nearest));
    Quantity_Color col(0.6, 0, 0.8, Quantity_TOC_RGB);
    Handle(Prs3d_LineAspect) aspect = new Prs3d_LineAspect(col, Aspect_TOL_DASH, 2.0);
    line->SetColor(col);
    line->Attributes()->SetLineAspect(aspect);
    m_context->Display(line, Standard_False);
    m_myObjects.append(line);
}



void WinPointLineRelation::onComputeLine()
{
    gp_Pnt p1, p2, target;
    if (!parsePoint(m_lineStartX, m_lineStartY, m_lineStartZ, p1, QString::fromLocal8Bit("起点"))) return;
    if (!parsePoint(m_lineEndX, m_lineEndY, m_lineEndZ, p2, QString::fromLocal8Bit("终点"))) return;
    if (!parsePoint(m_pointX_Line, m_pointY_Line, m_pointZ_Line, target, QString::fromLocal8Bit("目标点"))) return;

    bool seg = m_radioSegment->isChecked();
    drawLine(p1, p2, seg);

    ComputeResult res;
    if (m_radioNearest->isChecked())
        res = computeNearestPoint(p1, p2, target, seg);
    else if (m_radioSymmetric->isChecked())
        res = computeSymmetricPoint(p1, p2, target);
    else if (m_radioOffset->isChecked())
    {
        double d;
        if (!parseDouble(m_offsetDistance, d, QString::fromLocal8Bit("偏移距离"))) return;
        res = computeOffsetPoint(p1, p2, target, d);
    }

    drawPoint(target, Quantity_NOC_RED, QString::fromLocal8Bit("目标点"));
    drawPoint(res.point, res.color, res.description);
    drawPerpendicular(target, res.point);

    if (seg && (res.tRatio < 0 || res.tRatio > 1))
        drawExtension(res.tRatio < 0 ? p1 : p2, res.point);

    QString info = QString("%1: (%2, %3, %4)")
        .arg(res.description)
        .arg(QString::number(res.point.X(), 'f', 3))
        .arg(QString::number(res.point.Y(), 'f', 3))
        .arg(QString::number(res.point.Z(), 'f', 3));
    m_labelResultLine->setText(info);
    m_context->UpdateCurrentViewer();
}

void WinPointLineRelation::onComputeArc()
{
    gp_Pnt center, target;
    if (!parsePoint(m_arcCenterX, m_arcCenterY, m_arcCenterZ, center, QString::fromLocal8Bit("圆心"))) return;
    if (!parsePoint(m_pointX_Arc, m_pointY_Arc, m_pointZ_Arc, target, QString::fromLocal8Bit("目标点"))) return;

    double r, s, e;
    if (!parseDouble(m_arcRadius, r, QString::fromLocal8Bit("半径"))) return;
    if (!parseDouble(m_arcStartAngle, s, QString::fromLocal8Bit("起始角"))) return;
    if (!parseDouble(m_arcEndAngle, e, QString::fromLocal8Bit("终止角"))) return;

    drawArc(center, r, s * M_PI / 180, e * M_PI / 180);

    auto res = computeNearestPointArc(center, r, s * M_PI / 180, e * M_PI / 180, target);

    drawPoint(target, Quantity_NOC_RED, QString::fromLocal8Bit("目标点"));
    drawPoint(res.point, res.color, res.description);
    drawPerpendicular(target, res.point);

    QString info = QString("%1: (%2, %3, %4)")
        .arg(res.description)
        .arg(QString::number(res.point.X(), 'f', 3))
        .arg(QString::number(res.point.Y(), 'f', 3))
        .arg(QString::number(res.point.Z(), 'f', 3));
    m_labelResultArc->setText(info);
    m_context->UpdateCurrentViewer();
}

void WinPointLineRelation::onComputeBSpline()
{
    std::vector<gp_Pnt> pts(4);
    if (!parsePoint(m_bsplineP1X, m_bsplineP1Y, m_bsplineP1Z, pts[0], QString::fromLocal8Bit("控制点1"))) return;
    if (!parsePoint(m_bsplineP2X, m_bsplineP2Y, m_bsplineP2Z, pts[1], QString::fromLocal8Bit("控制点2"))) return;
    if (!parsePoint(m_bsplineP3X, m_bsplineP3Y, m_bsplineP3Z, pts[2], QString::fromLocal8Bit("控制点3"))) return;
    if (!parsePoint(m_bsplineP4X, m_bsplineP4Y, m_bsplineP4Z, pts[3], QString::fromLocal8Bit("控制点4"))) return;

    gp_Pnt target;
    if (!parsePoint(m_pointX_BSpline, m_pointY_BSpline, m_pointZ_BSpline, target, QString::fromLocal8Bit("目标点"))) return;

    drawBSpline(pts);

    auto res = computeNearestPointBSpline(pts, target);

    drawPoint(target, Quantity_NOC_RED, QString::fromLocal8Bit("目标点"));
    drawPoint(res.point, res.color, res.description);
    drawPerpendicular(target, res.point);

    QString info = QString("%1: (%2, %3, %4)")
        .arg(res.description)
        .arg(QString::number(res.point.X(), 'f', 3))
        .arg(QString::number(res.point.Y(), 'f', 3))
        .arg(QString::number(res.point.Z(), 'f', 3));
    m_labelResultBSpline->setText(info);
    m_context->UpdateCurrentViewer();
}



bool WinPointLineRelation::parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip)
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

bool WinPointLineRelation::parseDouble(QLineEdit* edit, double& value, const QString& tip)
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

void WinPointLineRelation::onClear()
{
    for (const auto& obj : m_myObjects)
        m_context->Remove(obj, Standard_False);
    m_myObjects.clear();
    m_context->UpdateCurrentViewer();

    m_labelResultLine->clear();
    m_labelResultArc->clear();
    m_labelResultBSpline->clear();
}



WinPointLineRelation::ComputeResult WinPointLineRelation::computeNearestPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target, bool asSegment)
{
    gp_Pnt res;
    double t = 0;
    if (asSegment)
    {
        gp_Vec v(p1, p2);
        gp_Vec w(p1, target);
        t = w.Dot(v) / v.SquareMagnitude();
        if (t <= 0) res = p1;
        else if (t >= 1) res = p2;
        else res = p1.Translated(v * t);
    }
    else
    {
        gp_Dir dir(gp_Vec(p1, p2));
        gp_Vec w(p1, target);
        t = w.Dot(dir);
        res = p1.Translated(dir.XYZ() * t);
    }
    return { res, QString::fromLocal8Bit("最近点"), Quantity_NOC_GREEN, t };

    //方法2 
    //// 用GeomAPI_ProjectPointOnCurve 投影器
    //gp_Pnt res;
    //double t = 0;
    //if (asSegment)
    //{
    //    Handle(Geom_Line) line=new Geom_Line(p1,gp_Dir(gp_Vec(p1,p2)));
    //    Handle(Geom_TrimmedCurve) segLine=new Geom_TrimmedCurve(line, 0,p1.Distance(p2));
    //    GeomAPI_ProjectPointOnCurve projector(target, segLine);
    //    res=projector.NearestPoint();
    //}
    //else
    //{
    //    Handle(Geom_Line) line = new Geom_Line(p1, gp_Dir(gp_Vec(p1, p2)));
    //    GeomAPI_ProjectPointOnCurve projector(target, line);
    //    res = projector.NearestPoint();
    //}
    //return { res, QString::fromLocal8Bit("最近点"), Quantity_NOC_GREEN, t }; 

    //方法3
     /*   if (asSegment)
        {
            Handle(Geom_Line) line=new Geom_Line(p1,gp_Dir(gp_Vec(p1,p2)));
            Handle(Geom_TrimmedCurve) segLine=new Geom_TrimmedCurve(line, 0,p1.Distance(p2));

            GeomAdaptor_Curve adaptor(segLine);
            Extrema_ExtPC extrema(target, adaptor);
        
            if (extrema.IsDone() && extrema.NbExt() > 0)
            {
                double minDist2 = RealLast();
                gp_Pnt nearest;
                double param = 0;
                for (int i = 1; i <= extrema.NbExt(); ++i)
                {
                    double d2 = extrema.SquareDistance(i);
                    if (d2 < minDist2)
                    {
                        minDist2 = d2;
                        nearest = extrema.Point(i).Value();   
                        param = extrema.Point(i).Parameter(); 
                        res=nearest;
                    }
                }
            }
        }
        else
        {
            Handle(Geom_Line) line = new Geom_Line(p1, gp_Dir(gp_Vec(p1, p2)));
            GeomAdaptor_Curve adaptor(line);
            Extrema_ExtPC extrema(target, adaptor);

            if (extrema.IsDone() && extrema.NbExt() > 0)
            {
                double minDist2 = RealLast();
                gp_Pnt nearest;
                double param = 0;
                for (int i = 1; i <= extrema.NbExt(); ++i)
                {
                    double d2 = extrema.SquareDistance(i);
                    if (d2 < minDist2)
                    {
                        minDist2 = d2;
                        nearest = extrema.Point(i).Value();
                        param = extrema.Point(i).Parameter();
                        res = nearest;
                    }
                }
            }
        }*/


    //方法4
    //if (asSegment)
    //{
    //    // 构造 Edge（线段）
    //    TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(p1, p2);
    //    // 构造 Vertex
    //    TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(target);
    //    BRepExtrema_DistShapeShape distShapeShape(vertex, edge);
    //    distShapeShape.Perform();
    //    if (distShapeShape.IsDone() && distShapeShape.NbSolution() > 0)
    //    {
    //        // 取第一个最近点
    //        gp_Pnt pOnEdge = distShapeShape.PointOnShape2(1);
    //        res = pOnEdge;

    //        // 如果需要 t，手动根据比例计算
    //        gp_Vec v(p1, p2);
    //        double length = v.Magnitude();
    //        if (length > Precision::Confusion())
    //        {
    //            gp_Vec w(p1, pOnEdge);
    //            t = w.Dot(v) / (length * length);
    //        }
    //    }
    //    else
    //    {
    //        // 失败则返回p1
    //        res = p1;
    //        t = 0.0;
    //    }
    //}
    //else
    //{
    //    // 仍使用直线投影逻辑
    //    gp_Dir dir(gp_Vec(p1, p2));
    //    gp_Vec w(p1, target);
    //    t = w.Dot(dir);
    //    res = p1.Translated(dir.XYZ() * t);
    //}


    //方法5
    // 创建无限直线
    //Handle(Geom_Line) geomLine = new Geom_Line(gp_Lin(p1, gp_Dir(gp_Vec(p1, p2))) );

    //ShapeAnalysis_Curve sac;
    //gp_Pnt projected;
    //Standard_Real param;

    //Standard_Real distance = sac.Project(
    //    geomLine,          // 曲线
    //    target,            // 投影点
    //    Precision::Confusion(), // 精度
    //    projected,         // 输出：投影点
    //    param,             // 输出：参数
    //    Standard_True      // 接近端点时自动吸附
    //);

    //// 线段长度
    //double segLen = p1.Distance(p2);
    //if (segLen < Precision::Confusion())
    //{
    //    res = p1;
    //    t = 0;
    //}
    //else
    //{
    //    // 如果是线段，裁剪
    //    if (asSegment)
    //    {
    //        if (param < 0.0)
    //            param = 0.0;
    //        else if (param > segLen)
    //            param = segLen;
    //    }

    //    geomLine->D0(param, res);
    //    t = param / segLen;
    //}

    //return { res, QString::fromLocal8Bit("最近点"), Quantity_NOC_GREEN, t }; 
}

WinPointLineRelation::ComputeResult WinPointLineRelation::computeSymmetricPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target)
{
    auto nearest = computeNearestPoint(p1, p2, target, true);
    gp_Vec v(nearest.point, target);
    gp_Pnt sym = nearest.point.Translated(v * 2);
    return { sym, QString::fromLocal8Bit("对称点"), Quantity_NOC_YELLOW };
}

WinPointLineRelation::ComputeResult WinPointLineRelation::computeOffsetPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target, double dist)
{
    auto nearest = computeNearestPoint(p1, p2, target, true);
    gp_Vec dir(nearest.point, target);
    if (dir.Magnitude() == 0)
        dir = gp_Vec(0, 0, 1);
    dir.Normalize();
    gp_Pnt offset = nearest.point.Translated(dir * dist);
    return { offset, QString::fromLocal8Bit("偏移点"), Quantity_NOC_BLUE };
}

WinPointLineRelation::ComputeResult WinPointLineRelation::computeNearestPointArc(const gp_Pnt& center, double radius, double startAngle, double endAngle, const gp_Pnt& target)
{
    gp_Ax2 plane(center, gp_Dir(0, 0, 1));
    Handle(Geom_Circle) circle = new Geom_Circle(plane, radius);
    Handle(Geom_TrimmedCurve) arc = new Geom_TrimmedCurve(circle, startAngle, endAngle);
    GeomAPI_ProjectPointOnCurve proj(target, arc);
    gp_Pnt nearest = proj.NearestPoint();
    return { nearest, QString::fromLocal8Bit("最近点"), Quantity_NOC_GREEN };
}

WinPointLineRelation::ComputeResult WinPointLineRelation::computeNearestPointBSpline(const std::vector<gp_Pnt>& pts, const gp_Pnt& target)
{
    // 创建插值曲线，确保通过所有控制点
    Handle(TColgp_HArray1OfPnt) hPts = new TColgp_HArray1OfPnt(1, Standard_Integer(pts.size()));
    for (Standard_Integer i = 1; i <= pts.size(); ++i)
        hPts->SetValue(i, pts[i - 1]);

    GeomAPI_Interpolate interpolator(hPts, Standard_False, 1.0e-6);
    interpolator.Perform();
    Handle(Geom_BSplineCurve) bspline = interpolator.Curve();

    // 投影目标点
    GeomAPI_ProjectPointOnCurve proj(target, bspline);
    gp_Pnt nearest = proj.NearestPoint();
    return { nearest, QString::fromLocal8Bit("最近点"), Quantity_NOC_GREEN };

}
