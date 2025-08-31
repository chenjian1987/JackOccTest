#pragma once
// ===================== WinArcFaceBSurfIntersect.h =======================
// Qt 对话框：圆弧曲面 × B样条面相交测试
// ====================================================================

#include <QDialog>
#include <QTabWidget>
#include <QFormLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Point.hxx>
#include <AIS_TextLabel.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Prs3d_LineAspect.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomAPI_PointsToBSplineSurface.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <GeomAPI_IntSS.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAPI_IntCS.hxx>
#include <BRepTools.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopExp_Explorer.hxx>

#include "OwnerPtr.h"
#include "FreeFaceIntersectFreeFace.h"



using namespace JackC;

class WinArcFaceIntersectBSplineFace : public QDialog
{
    Q_OBJECT
public:
    WinArcFaceIntersectBSplineFace(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer, Handle(V3d_View) v3dView,  QWidget* parent,  std::function<void(const QString&)> func);

private slots:
    void onCompute();
    void onClear();

private:
    void setupUI();
    QWidget* makeXYZRow(const QString& title,
        QLineEdit*& ex, QLineEdit*& ey, QLineEdit*& ez,
        const QString& x0, const QString& y0, const QString& z0);
    QWidget* makeCtrlRow(const QString& name,
        QLineEdit*& ex, QLineEdit*& ey, QLineEdit*& ez,
        const gp_Pnt& P);

    void drawArcFace(const gp_Pnt& center, double radius, double startAngle, double endAngle, double extrusion);
    void drawBSplineFace(const Handle(Geom_BSplineSurface)& surf);
    void drawEdges(const std::vector<TopoDS_Edge>& edges, Quantity_NameOfColor color);

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer)             m_v3dViewer;
    Handle(V3d_View)               m_v3dView;
    QList<Handle(AIS_InteractiveObject)> m_myObjects;

    // 圆弧曲面输入
    QLineEdit* m_arcCX, * m_arcCY, * m_arcCZ;
    QLineEdit* m_arcR, * m_arcS, * m_arcE;
    QLineEdit* m_arcExtrusion; // 拉伸距离

    // B 样条面控制点
    QLineEdit* m_s11x, * m_s11y, * m_s11z, * m_s12x, * m_s12y, * m_s12z, * m_s13x, * m_s13y, * m_s13z;
    QLineEdit* m_s21x, * m_s21y, * m_s21z, * m_s22x, * m_s22y, * m_s22z, * m_s23x, * m_s23y, * m_s23z;
    QLineEdit* m_s31x, * m_s31y, * m_s31z, * m_s32x, * m_s32y, * m_s32z, * m_s33x, * m_s33y, * m_s33z;

    // 参数 & 方法
    QComboBox* m_comboMethod;
    QDoubleSpinBox* m_spinTol3d;
    QDoubleSpinBox* m_spinTolNear;
    QSpinBox* m_spinSamples;
    QCheckBox* m_chkRefine;
    QPushButton* m_btnCompute;
    QPushButton* m_btnClear;
    QLabel* m_labelInfo;

    OwnerPtr<FreeFaceIntersectFreeFace> m_util;
    std::function<void(const QString&)> m_outputFunc;
};