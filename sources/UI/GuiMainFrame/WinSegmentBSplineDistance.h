#ifndef WIN_SEGMENT_BSPLINE_DISTANCE_H
#define WIN_SEGMENT_BSPLINE_DISTANCE_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QComboBox>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>

#include "GeomLineSegmentRelationUtility.h"
#include "OwnerPtr.h"

class AIS_Shape;
class AIS_Point;
class AIS_TextLabel;

using namespace JackC;


class WinSegmentBSplineDistance : public QDialog
{
    Q_OBJECT

public:
    WinSegmentBSplineDistance(Handle(AIS_InteractiveContext) context, Handle(V3d_Viewer) v3dViewer,  Handle(V3d_View) v3dView, QWidget* parent = nullptr, std::function<void(const QString&)> outputFunc = {});

private slots:
    void onCompute();
    void onClear();

private:
    void setupUI();

    QWidget* createPointInput(const QString& labelText,  QLineEdit*& px,  QLineEdit*& py, QLineEdit*& pz,
        const QString& dx,const QString& dy,  const QString& dz);

    bool parsePoint(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez,  gp_Pnt& p, const QString& tip);

    void drawSegment(const gp_Pnt& p1, const gp_Pnt& p2, Quantity_NameOfColor col);

    void drawBSpline(const std::vector<gp_Pnt>& controlPoints);

    void drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name);

    void drawConnectingSegment(const gp_Pnt& p1, const gp_Pnt& p2);

    void computeNearest(const gp_Pnt& p1, const gp_Pnt& p2,   const std::vector<gp_Pnt>& ctrlPts,  gp_Pnt& nearOnLine,  gp_Pnt& nearOnSpline,   int method);

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;
    std::function<void(const QString&)> m_outputFunc;
    QList<Handle(AIS_InteractiveObject)> m_objects;

    // Input fields for line segment endpoints
    QLineEdit* m_p1x;
    QLineEdit* m_p1y;
    QLineEdit* m_p1z;
    QLineEdit* m_p2x;
    QLineEdit* m_p2y;
    QLineEdit* m_p2z;
    // Input fields for B‑spline control points
    QLineEdit* m_c1x;
    QLineEdit* m_c1y;
    QLineEdit* m_c1z;
    QLineEdit* m_c2x;
    QLineEdit* m_c2y;
    QLineEdit* m_c2z;
    QLineEdit* m_c3x;
    QLineEdit* m_c3y;
    QLineEdit* m_c3z;
    QLineEdit* m_c4x;
    QLineEdit* m_c4y;
    QLineEdit* m_c4z;

    QPushButton* m_btnCompute;
    QPushButton* m_btnClear;
    QLabel* m_labelResult;

    QComboBox* m_comboMethod;
    enum Method 
    {
        METHOD_ExtremaCurveCurve = 0,
        METHOD_DistShapeShape,
        METHOD_ProjectPointOnCurve,
        METHOD_ShapeAnalysisProject,
    };

    OwnerPtr< GeomLineSegmentRelationUtility> m_opUti;
};

#endif // WIN_SEGMENT_BSPLINE_DISTANCE_H