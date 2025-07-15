#ifndef WINPOINTONSURFACE_H
#define WINPOINTONSURFACE_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <Precision.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <functional>





class WinPointOnSurface : public QDialog
{
    Q_OBJECT

public:
    WinPointOnSurface(Handle(AIS_InteractiveContext) context,
        Handle(V3d_Viewer) v3dViewer,
        Handle(V3d_View) v3dView,
        QWidget* parent = nullptr,
        std::function<void(const QString&)> func = nullptr);

private:
    void setupUI();
    QWidget* createPointInputLayout(QLineEdit*& x, QLineEdit*& y, QLineEdit*& z);

    void setupPlaneTab(QWidget* tab);
    void setupSphereTab(QWidget* tab);
    void setupCylinderTab(QWidget* tab);
    void setupBSplineTab(QWidget* tab);

    void drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name);
    void drawPlane(const gp_Pnt& point, const gp_Dir& normal);
    void drawSphere(const gp_Pnt& center, double radius);
    void drawCylinder(const gp_Pnt& axisPoint, const gp_Dir& axisDir, double radius, double height);
    void drawBSplineSurface(const std::vector<gp_Pnt>& controlPoints);

    bool parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip);
    bool parseDouble(QLineEdit* edit, double& value, const QString& tip);
    std::vector<gp_Pnt> parseControlPoints(const QString& input);

    bool checkPointOnPlane(const gp_Pnt& point, double A, double B, double C, double D);
    bool checkPointOnSphere(const gp_Pnt& point, const gp_Pnt& center, double radius);
    bool checkPointOnCylinder(const gp_Pnt& point, const gp_Pnt& axisPoint, const gp_Dir& axisDir, double radius);
    bool checkPointOnBSplineSurface(const gp_Pnt& point, const std::vector<gp_Pnt>& controlPoints);

private slots:
    void onComputePlane();
    void onComputeSphere();
    void onComputeCylinder();
    void onComputeBSpline();
    void onClear();

private:
    // OCCT 上下文和视图
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_v3dView;
    Handle(V3d_Viewer) m_v3dViewer;
    std::function<void(const QString&)> m_outputFunc;

    // 存储创建的图形对象
    QList<Handle(AIS_InteractiveObject)> m_myObjects;

    // 平面标签页控件
    QLineEdit* m_pointX_plane, * m_pointY_plane, * m_pointZ_plane;
    QLineEdit* m_planeA, * m_planeB, * m_planeC, * m_planeD;
    QPushButton* m_btnComputePlane, * m_btnClearPlane;
    QLabel* m_labelResultPlane;

    // 球面标签页控件
    QLineEdit* m_pointX_sphere, * m_pointY_sphere, * m_pointZ_sphere;
    QLineEdit* m_sphereCenterX, * m_sphereCenterY, * m_sphereCenterZ;
    QLineEdit* m_sphereRadius;
    QPushButton* m_btnComputeSphere, * m_btnClearSphere;
    QLabel* m_labelResultSphere;

    // 圆柱面标签页控件
    QLineEdit* m_pointX_cylinder, * m_pointY_cylinder, * m_pointZ_cylinder;
    QLineEdit* m_cylinderAxisX, * m_cylinderAxisY, * m_cylinderAxisZ;
    QLineEdit* m_cylinderRadius, * m_cylinderHeight;
    QPushButton* m_btnComputeCylinder, * m_btnClearCylinder;
    QLabel* m_labelResultCylinder;

    // B样条曲面标签页控件
    QLineEdit* m_pointX_bspline, * m_pointY_bspline, * m_pointZ_bspline;
    QLineEdit* m_bsplineControlPoints;
    QPushButton* m_btnComputeBSpline, * m_btnClearBSpline;
    QLabel* m_labelResultBSpline;

    // 标签页
    QWidget* m_tabPlane, * m_tabSphere, * m_tabCylinder, * m_tabBSpline;
};

#endif // WINPOINTONSURFACE_H