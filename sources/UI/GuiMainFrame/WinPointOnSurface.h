#ifndef WINPOINTONSURFACE_H
#define WINPOINTONSURFACE_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QComboBox>
#include <AIS_InteractiveContext.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <Precision.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <functional>
#include <Geom_Plane.hxx>
#include <Geom_BSplineSurface.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <Extrema_ExtPS.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <GeomLProp_SLProps.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopAbs_State.hxx>


#include "GeomPointPlanRelationshipUtility.h"
#include "GeomPointSurfaceRelationshipUtility.h"
#include "OwnerPtr.h"



using namespace JackC;



// 计算方法的枚举
enum ComputationMethod {
    METHOD_DistanceInterface,        // 距离接口
    METHOD_Math,                     // 数学方式
    METHOD_Projector,                // 投影器
    METHOD_AnalysisTool,             // 分析工具
    METHOD_Extrema,                  // 极值计算
    METHOD_GeomAdaptor,              // GeomAdaptor
    METHOD_FaceClassifier,           // 面域分类器
    METHOD_DistShapeShape            // 距离计算
};

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

    // 平面检查的多种方法
    bool checkPointOnPlane(const gp_Pnt& point, double A, double B, double C, double D, int method, bool& isOnPlane, gp_Pnt& nearPoint, double& distance);

    // 其他曲面类型的检查方法
    bool checkPointOnSphere(const gp_Pnt& point, const gp_Pnt& center, double radius, int method, bool& isOnSphere, gp_Pnt& nearPoint, double& distance);
    bool checkPointOnCylinder(const gp_Pnt& point, const gp_Pnt& axisPoint, const gp_Dir& axisDir,
            double radius, int method, bool& isOnCylinder, gp_Pnt& nearPoint, double& distance);
    bool checkPointOnBSplineSurface(const gp_Pnt& point, const std::vector<gp_Pnt>& controlPoints, int method,
        bool& isOnBSpline, gp_Pnt& nearPoint, double& distance);

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
    QComboBox* m_comboMethodPlane;  

    // 球面标签页控件
    QLineEdit* m_pointX_sphere, * m_pointY_sphere, * m_pointZ_sphere;
    QLineEdit* m_sphereCenterX, * m_sphereCenterY, * m_sphereCenterZ;
    QLineEdit* m_sphereRadius;
    QPushButton* m_btnComputeSphere, * m_btnClearSphere;
    QLabel* m_labelResultSphere;
    QComboBox* m_comboMethodSphere;  

    // 圆柱面标签页控件
    QLineEdit* m_pointX_cylinder, * m_pointY_cylinder, * m_pointZ_cylinder;
    QLineEdit* m_cylinderAxisX, * m_cylinderAxisY, * m_cylinderAxisZ;
    QLineEdit* m_cylinderRadius, * m_cylinderHeight;
    QPushButton* m_btnComputeCylinder, * m_btnClearCylinder;
    QLabel* m_labelResultCylinder;
    QComboBox* m_comboMethodCylinder;  

    // B样条曲面标签页控件
    QLineEdit* m_pointX_bspline, * m_pointY_bspline, * m_pointZ_bspline;
    QLineEdit* m_bsplineControlPoints;
    QPushButton* m_btnComputeBSpline, * m_btnClearBSpline;
    QLabel* m_labelResultBSpline;
    QComboBox* m_comboMethodBSpline;  

    // 标签页
    QWidget* m_tabPlane, * m_tabSphere, * m_tabCylinder, * m_tabBSpline;


private:
    OwnerPtr<GeomPointPlanRelationshipUtility> op_pointPlaneRelationshipUtil;
    OwnerPtr<GeomPointSurfaceRelationshipUtility> op_pointSurfaceRelationUtil;

};

#endif // WINPOINTONSURFACE_H