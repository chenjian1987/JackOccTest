#include "ShapeEditDialog.h"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>





ShapeEditDialog::ShapeEditDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(QString::fromStdWString(L"形体变换参数"));
    setMinimumWidth(320);

    typeCombo = new QComboBox();
    typeCombo->addItem(QString::fromStdWString(L"平移"), Translate);
    typeCombo->addItem(QString::fromStdWString(L"旋转"), Rotate);
    typeCombo->addItem(QString::fromStdWString(L"镜像"), Mirror);

    xSpin = new QDoubleSpinBox(); xSpin->setRange(-1e6, 1e6); xSpin->setDecimals(3); xSpin->setSuffix(" X");
    ySpin = new QDoubleSpinBox(); ySpin->setRange(-1e6, 1e6); ySpin->setDecimals(3); ySpin->setSuffix(" Y");
    zSpin = new QDoubleSpinBox(); zSpin->setRange(-1e6, 1e6); zSpin->setDecimals(3); zSpin->setSuffix(" Z");
    valueSpin = new QDoubleSpinBox(); valueSpin->setRange(-1e6, 1e6); valueSpin->setDecimals(3);

    mirrorPlaneCombo = new QComboBox();
    mirrorPlaneCombo->addItem(QString::fromStdWString(L"YZ面"), 0);
    mirrorPlaneCombo->addItem(QString::fromStdWString(L"XZ面"), 1);
    mirrorPlaneCombo->addItem(QString::fromStdWString(L"XY面"), 2);

    okBtn = new QPushButton(QString::fromStdWString(L"确定"));
    cancelBtn = new QPushButton(QString::fromStdWString(L"取消"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel(QString::fromStdWString(L"操作类型:")));
    typeLayout->addWidget(typeCombo);
    mainLayout->addLayout(typeLayout);

    mainLayout->addWidget(new QLabel(QString::fromStdWString(L"参数(向量/轴/平面):")));
    QHBoxLayout* vecLayout = new QHBoxLayout();
    vecLayout->addWidget(xSpin);
    vecLayout->addWidget(ySpin);
    vecLayout->addWidget(zSpin);
    mainLayout->addLayout(vecLayout);

    mainLayout->addWidget(valueSpin);
    mainLayout->addWidget(mirrorPlaneCombo);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(typeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
    connect(okBtn, &QPushButton::clicked, this, &ShapeEditDialog::onOk);
    connect(cancelBtn, &QPushButton::clicked, this, &ShapeEditDialog::reject);

    onTypeChanged(0);
}




void ShapeEditDialog::onTypeChanged(int idx)
{
    int t = typeCombo->currentData().toInt();
    bool isTrans = (t == Translate);
    bool isRot = (t == Rotate);
    bool isMirror = (t == Mirror);

    xSpin->setVisible(isTrans || isRot);
    ySpin->setVisible(isTrans || isRot);
    zSpin->setVisible(isTrans || isRot);

    valueSpin->setVisible(isTrans || isRot);
    valueSpin->setPrefix(isTrans ? QString::fromStdWString(L"距离: ") : QString::fromStdWString(L"角度: "));
    valueSpin->setSuffix(isTrans ? QString::fromStdWString(L" mm") : QString::fromStdWString(L" °"));

    mirrorPlaneCombo->setVisible(isMirror);
}

void ShapeEditDialog::onOk()
{
    accept();
}



ShapeEditDialog::TransformType ShapeEditDialog::getType() const
{
    return (TransformType)typeCombo->currentData().toInt();
}



QVector3D ShapeEditDialog::getVector() const
{
    return QVector3D(xSpin->value(), ySpin->value(), zSpin->value());
}

double ShapeEditDialog::getValue() const
{
    return valueSpin->value();
}

int ShapeEditDialog::getMirrorPlane() const
{
    return mirrorPlaneCombo->currentData().toInt();
}
