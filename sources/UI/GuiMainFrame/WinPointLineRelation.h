#ifndef WINPOINTLINERELATION_H
#define WINPOINTLINERELATION_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QLabel>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Point.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>




class WinPointLineRelation : public QDialog
{
    Q_OBJECT
public:
    WinPointLineRelation(
        Handle(AIS_InteractiveContext) context,
        Handle(V3d_Viewer) v3dViewer,
        Handle(V3d_View) v3dView,
        QWidget* parent,
        std::function<void(const QString&)> func
    );

private slots:
    void onComputeLine();
    void onComputeArc();
    void onComputeBSpline();
    void onClear();

private:
    struct ComputeResult
    {
        gp_Pnt point;
        QString description;
        Quantity_NameOfColor color;
        double tRatio = 0.0;
    };

    void setupUI();
    void setupLineTab(QWidget* tab);
    void setupArcTab(QWidget* tab);
    void setupBSplineTab(QWidget* tab);

    bool parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip);
    bool parseDouble(QLineEdit* edit, double& value, const QString& tip);

    void drawLine(const gp_Pnt& p1, const gp_Pnt& p2, bool asSegment);
    void drawArc(const gp_Pnt& center, double radius, double startAngle, double endAngle);
    void drawBSpline(const std::vector<gp_Pnt>& controlPoints);
    void drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name);
    void drawPerpendicular(const gp_Pnt& target, const gp_Pnt& nearest);
    void drawExtension(const gp_Pnt& endpoint, const gp_Pnt& nearest);

    ComputeResult computeNearestPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target, bool asSegment);
    ComputeResult computeSymmetricPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target);
    ComputeResult computeOffsetPoint(const gp_Pnt& p1, const gp_Pnt& p2, const gp_Pnt& target, double dist);
    ComputeResult computeNearestPointArc(const gp_Pnt& center, double radius, double startAngle, double endAngle, const gp_Pnt& target);
    ComputeResult computeNearestPointBSpline(const std::vector<gp_Pnt>& controlPoints, const gp_Pnt& target);

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;
    QList<Handle(AIS_InteractiveObject)> m_myObjects;

    // --- 直线/线段 ---
    QLineEdit* m_lineStartX; QLineEdit* m_lineStartY; QLineEdit* m_lineStartZ;
    QLineEdit* m_lineEndX;   QLineEdit* m_lineEndY;   QLineEdit* m_lineEndZ;
    QLineEdit* m_pointX_Line; QLineEdit* m_pointY_Line; QLineEdit* m_pointZ_Line;
    QLineEdit* m_offsetDistance;
    QRadioButton* m_radioLine;
    QRadioButton* m_radioSegment;
    QRadioButton* m_radioNearest;
    QRadioButton* m_radioSymmetric;
    QRadioButton* m_radioOffset;
    QPushButton* m_btnComputeLine;
    QPushButton* m_btnClearLine;
    QLabel* m_labelResultLine;

    // --- 圆弧 ---
    QLineEdit* m_arcCenterX; QLineEdit* m_arcCenterY; QLineEdit* m_arcCenterZ;
    QLineEdit* m_arcRadius;
    QLineEdit* m_arcStartAngle;
    QLineEdit* m_arcEndAngle;
    QLineEdit* m_pointX_Arc; QLineEdit* m_pointY_Arc; QLineEdit* m_pointZ_Arc;
    QPushButton* m_btnComputeArc;
    QPushButton* m_btnClearArc;
    QLabel* m_labelResultArc;

    // --- B样条 ---
    QLineEdit* m_bsplineP1X; QLineEdit* m_bsplineP1Y; QLineEdit* m_bsplineP1Z;
    QLineEdit* m_bsplineP2X; QLineEdit* m_bsplineP2Y; QLineEdit* m_bsplineP2Z;
    QLineEdit* m_bsplineP3X; QLineEdit* m_bsplineP3Y; QLineEdit* m_bsplineP3Z;
    QLineEdit* m_bsplineP4X; QLineEdit* m_bsplineP4Y; QLineEdit* m_bsplineP4Z;
    QLineEdit* m_pointX_BSpline; QLineEdit* m_pointY_BSpline; QLineEdit* m_pointZ_BSpline;
    QPushButton* m_btnComputeBSpline;
    QPushButton* m_btnClearBSpline;
    QLabel* m_labelResultBSpline;

    std::function<void(const QString&)> m_outputFunc;
};

#endif // WINPOINTLINERELATION_H
