#ifndef MATRIX44OPERATIONDIALOG_H
#define MATRIX44OPERATIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <functional>
#include "Matrix44.h"
#include "CoordinateSystemDrawer.h"
#include <AIS_Point.hxx>

class Matrix44OperationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit Matrix44OperationDialog(Handle(AIS_InteractiveContext) context,
        Handle(V3d_Viewer) v3dViewer,
        Handle(V3d_View) v3dView,
        QWidget* parent = nullptr,
        std::function<void(const QString&)> func = nullptr);
    ~Matrix44OperationDialog();

private slots:
    void onApply();
    void onTypeChanged(int idx);
    void onPointInputChanged();  // 新增：点坐标变化时更新显示

private:
    void applyMatrixOperation();
    void updateCoordinateSystem(const Matrix44& newMatrix);
    void updateMatrixDisplay(const Matrix44& newMatrix);
    void updatePointDisplay(const Matrix44& matrix);  // 新增：更新点显示
    Matrix44 getInputMatrix();
    Matrix44 getSecondInputMatrix();
    Vector3d getInputPoint();  // 新增：获取输入点坐标

    // UI Components
    QComboBox* typeCombo;
    QDoubleSpinBox* xSpin, * ySpin, * zSpin, * angleSpin;
    QDoubleSpinBox* pointXSpin, * pointYSpin, * pointZSpin;  // 新增：点坐标输入
    QPushButton* applyBtn, * cancelBtn;
    QTextEdit* matrixOutputText;
    QDoubleSpinBox* matrixInputs[4][4];
    QDoubleSpinBox* matrixInputs2[4][4];

    CoordinateSystemDrawer* drawer;
    Matrix44 matrix;
    Matrix44 newMatrix;
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer) m_v3dViewer;
    Handle(V3d_View) m_v3dView;
    std::function<void(const QString&)> m_outputFunc;

    Handle(AIS_Trihedron) m_mat1Trihedron;
    Handle(AIS_Trihedron) m_ansMatTrihedron;

    // 新增：点可视化对象
    Handle(AIS_Point) m_originalPoint;
    Handle(AIS_Point) m_transformedPoint;
};

#endif // MATRIX44OPERATIONDIALOG_H