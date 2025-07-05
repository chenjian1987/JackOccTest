#pragma once
#include <QDialog>
#include <QVector3D>
#include <qmenu.h>

class QDoubleSpinBox;
class QPushButton;
class QLabel;




class ShapeEditDialog : public QDialog
{
    Q_OBJECT
public:
    ShapeEditDialog(QWidget* parent = nullptr);

public:
    // 操作类型
    enum TransformType { Translate, Rotate, Mirror };

    TransformType getType() const;
    QVector3D getVector() const; // 平移向量或旋转轴
    double getValue() const;     // 距离/角度
    int getMirrorPlane() const;  // 镜像平面（0:X, 1:Y, 2:Z）

private slots:
    void onTypeChanged(int);
    void onOk();

private:
    QComboBox* typeCombo;
    QDoubleSpinBox* xSpin;
    QDoubleSpinBox* ySpin;
    QDoubleSpinBox* zSpin;
    QDoubleSpinBox* valueSpin;
    QComboBox* mirrorPlaneCombo;
    QPushButton* okBtn;
    QPushButton* cancelBtn;

    void updateFields();


};
