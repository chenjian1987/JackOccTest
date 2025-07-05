#ifndef WINPARAMETRICBUILD_H
#define WINPARAMETRICBUILD_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <Geom_Circle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <AIS_Shape.hxx>
#include <gp_Pnt.hxx>



class WinParametricBuild : public QDialog
{
    Q_OBJECT
public:
    WinParametricBuild(
        Handle(AIS_InteractiveContext) context,
        Handle(V3d_Viewer) v3dViewer,
        Handle(V3d_View) v3dView,
        QWidget* parent = nullptr);

private slots:
    void onDrawLine();
    void onDrawCircle();
    void onDrawSphere();
    void onDrawBox();
    void onClear();

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;

    // 参数输入控件
    QLineEdit* m_lineStartX; QLineEdit* m_lineStartY; QLineEdit* m_lineStartZ;
    QLineEdit* m_lineEndX;   QLineEdit* m_lineEndY;   QLineEdit* m_lineEndZ;
    QLineEdit* m_circleX; QLineEdit* m_circleY; QLineEdit* m_circleZ; QLineEdit* m_circleRadius;
    QLineEdit* m_sphereX; QLineEdit* m_sphereY; QLineEdit* m_sphereZ; QLineEdit* m_sphereRadius;
    QLineEdit* m_boxMinX; QLineEdit* m_boxMinY; QLineEdit* m_boxMinZ;
    QLineEdit* m_boxMaxX; QLineEdit* m_boxMaxY; QLineEdit* m_boxMaxZ;

    // 绘制按钮
    QPushButton* m_btnDrawLine;
    QPushButton* m_btnDrawCircle;
    QPushButton* m_btnDrawSphere;
    QPushButton* m_btnDrawBox;
    QPushButton* m_btnClear;

    void setupUI();

    // 几何绘制
    void drawLine(const gp_Pnt& p1, const gp_Pnt& p2);
    void drawCircle(const gp_Pnt& center, double radius);
    void drawSphere(const gp_Pnt& center, double radius);
    void drawBox(const gp_Pnt& minPt, const gp_Pnt& maxPt);

    // 工具
    bool parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip);
    bool parseDouble(QLineEdit* edit, double& value, const QString& tip);
};

#endif // WINPARAMETRICBUILD_H
