#ifndef WIN_SEGMENT_DISTANCE_H
#define WIN_SEGMENT_DISTANCE_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <gp_Pnt.hxx>

#include "GeomLineSegmentRelationUtility.h"
#include "OwnerPtr.h"




using namespace JackC;

class WinSegmentDistance : public QDialog
{
    Q_OBJECT

public:
    explicit WinSegmentDistance(Handle(AIS_InteractiveContext) context,
        Handle(V3d_Viewer) v3dViewer,
        Handle(V3d_View) v3dView,
        QWidget* parent = nullptr,
        std::function<void(const QString&)> outputFunc = {});

private slots:
    void onClear();
    void onCompute();

private:
    void setupUI();

    QWidget* createPointInputLayout(QLineEdit*& x, QLineEdit*& y, QLineEdit*& z);

    bool parsePoint(QLineEdit* xEdit, QLineEdit* yEdit, QLineEdit* zEdit, gp_Pnt& pt, const QString& tip);

    // Draw a point in the viewer with a label.
    void drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& name);

    // Draw a line segment between two points in the viewer.
    void drawSegment(const gp_Pnt& p1, const gp_Pnt& p2, Quantity_NameOfColor color);

    // Draw a text label next to a 3D point. Used for result annotation.
    void drawLabel(const gp_Pnt& p, const QString& text, Quantity_NameOfColor color);

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;
    std::function<void(const QString&)> m_outputFunc;
    QList<Handle(AIS_InteractiveObject)> m_myObjects;

    // Point input fields for the first segment (P1 and P2).
    QLineEdit* m_p1x;
    QLineEdit* m_p1y;
    QLineEdit* m_p1z;
    QLineEdit* m_p2x;
    QLineEdit* m_p2y;
    QLineEdit* m_p2z;
    // Point input fields for the second segment (Q1 and Q2).
    QLineEdit* m_q1x;
    QLineEdit* m_q1y;
    QLineEdit* m_q1z;
    QLineEdit* m_q2x;
    QLineEdit* m_q2y;
    QLineEdit* m_q2z;

    // Combo box to select the algorithm/method.
    QComboBox* m_comboMethod;
    // Compute and clear buttons.
    QPushButton* m_btnCompute;
    QPushButton* m_btnClear;
    // Label to display the result.
    QLabel* m_labelResult;

    OwnerPtr<GeomLineSegmentRelationUtility> m_opUti;

    enum Method
    {
        METHOD_ExtremaCurve = 0,
        METHOD_DistShapeShape,
        METHOD_ProjectPointOnCurve,
        METHOD_ShapeAnalysisProject,
        METHOD_Manual
    };
};

#endif 