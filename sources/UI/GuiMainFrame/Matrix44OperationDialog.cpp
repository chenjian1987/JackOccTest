#include "Matrix44OperationDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <AIS_Point.hxx>
#include <Geom_CartesianPoint.hxx>

Matrix44OperationDialog::Matrix44OperationDialog(Handle(AIS_InteractiveContext) context,
    Handle(V3d_Viewer) v3dViewer,
    Handle(V3d_View) v3dView,
    QWidget* parent,
    std::function<void(const QString&)> func)
    : QDialog(parent), m_context(context), m_v3dViewer(v3dViewer), m_v3dView(v3dView),
    m_outputFunc(func), matrix(Matrix44()), drawer(nullptr)
{
    setWindowTitle(QString::fromLocal8Bit("Matrix44 操作"));
    setMinimumSize(900, 700);  // 增加窗口大小以容纳新控件

    // 初始化UI组件
    typeCombo = new QComboBox();
    typeCombo->addItem(QString::fromLocal8Bit("平移"), 0);
    typeCombo->addItem(QString::fromLocal8Bit("旋转"), 1);
    typeCombo->addItem(QString::fromLocal8Bit("逆矩阵"), 2);
    typeCombo->addItem(QString::fromLocal8Bit("左乘"), 3);
    typeCombo->addItem(QString::fromLocal8Bit("右乘"), 4);
    typeCombo->addItem(QString::fromLocal8Bit("矩阵与逆矩阵相乘"), 5);

    // 变换参数输入
    xSpin = new QDoubleSpinBox(); xSpin->setRange(-1e6, 1e6); xSpin->setDecimals(3);
    ySpin = new QDoubleSpinBox(); ySpin->setRange(-1e6, 1e6); ySpin->setDecimals(3);
    zSpin = new QDoubleSpinBox(); zSpin->setRange(-1e6, 1e6); zSpin->setDecimals(3);
    angleSpin = new QDoubleSpinBox(); angleSpin->setRange(0, 360); angleSpin->setDecimals(3);

    // 新增：点坐标输入
    pointXSpin = new QDoubleSpinBox(); pointXSpin->setRange(-1e6, 1e6); pointXSpin->setDecimals(3); pointXSpin->setValue(1.0);
    pointYSpin = new QDoubleSpinBox(); pointYSpin->setRange(-1e6, 1e6); pointYSpin->setDecimals(3); pointYSpin->setValue(0.0);
    pointZSpin = new QDoubleSpinBox(); pointZSpin->setRange(-1e6, 1e6); pointZSpin->setDecimals(3); pointZSpin->setValue(0.0);

    applyBtn = new QPushButton(QString::fromLocal8Bit("应用"));
    cancelBtn = new QPushButton(QString::fromLocal8Bit("取消"));

    // 创建主布局 - 垂直布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 第一行：矩阵1和矩阵2并排
    QHBoxLayout* matricesLayout = new QHBoxLayout();

    // 矩阵1
    QGroupBox* matrix1Group = new QGroupBox(QString::fromLocal8Bit("矩阵1"));
    QVBoxLayout* matrix1Layout = new QVBoxLayout(matrix1Group);
    QGridLayout* matrixGrid1 = new QGridLayout();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrixInputs[i][j] = new QDoubleSpinBox();
            matrixInputs[i][j]->setRange(-1e6, 1e6);
            matrixInputs[i][j]->setDecimals(3);
            matrixInputs[i][j]->setValue(i == j ? 1.0 : 0.0);
            matrixInputs[i][j]->setMinimumWidth(80);
            matrixGrid1->addWidget(matrixInputs[i][j], i, j);
        }
    }
    matrix1Layout->addLayout(matrixGrid1);
    matricesLayout->addWidget(matrix1Group);

    // 矩阵2
    QGroupBox* matrix2Group = new QGroupBox(QString::fromLocal8Bit("矩阵2"));
    QVBoxLayout* matrix2Layout = new QVBoxLayout(matrix2Group);
    QGridLayout* matrixGrid2 = new QGridLayout();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrixInputs2[i][j] = new QDoubleSpinBox();
            matrixInputs2[i][j]->setRange(-1e6, 1e6);
            matrixInputs2[i][j]->setDecimals(3);
            matrixInputs2[i][j]->setValue(i == j ? 1.0 : 0.0);
            matrixInputs2[i][j]->setMinimumWidth(80);
            matrixGrid2->addWidget(matrixInputs2[i][j], i, j);
        }
    }
    matrix2Layout->addLayout(matrixGrid2);
    matricesLayout->addWidget(matrix2Group);

    mainLayout->addLayout(matricesLayout);

    // 第二行：点坐标输入
    QGroupBox* pointGroup = new QGroupBox(QString::fromLocal8Bit("测试点坐标"));
    QHBoxLayout* pointLayout = new QHBoxLayout(pointGroup);
    pointLayout->addWidget(new QLabel(QString::fromLocal8Bit("X:")));
    pointLayout->addWidget(pointXSpin);
    pointLayout->addWidget(new QLabel(QString::fromLocal8Bit("Y:")));
    pointLayout->addWidget(pointYSpin);
    pointLayout->addWidget(new QLabel(QString::fromLocal8Bit("Z:")));
    pointLayout->addWidget(pointZSpin);
    pointLayout->addStretch();
    mainLayout->addWidget(pointGroup);

    // 第三行：操作选择
    QGroupBox* operationGroup = new QGroupBox(QString::fromLocal8Bit("选择"));
    QHBoxLayout* operationLayout = new QHBoxLayout(operationGroup);

    operationLayout->addWidget(new QLabel(QString::fromLocal8Bit("操作类型:")));
    operationLayout->addWidget(typeCombo);
    operationLayout->addStretch();

    // 参数输入区域
    QWidget* paramsWidget = new QWidget();
    QHBoxLayout* paramsLayout = new QHBoxLayout(paramsWidget);
    paramsLayout->addWidget(new QLabel(QString::fromLocal8Bit("向量:")));
    paramsLayout->addWidget(xSpin);
    paramsLayout->addWidget(ySpin);
    paramsLayout->addWidget(zSpin);
    paramsLayout->addWidget(new QLabel(QString::fromLocal8Bit("角度:")));
    paramsLayout->addWidget(angleSpin);
    paramsLayout->addStretch();

    operationLayout->addWidget(paramsWidget);
    mainLayout->addWidget(operationGroup);

    // 第四行：结果显示
    QGroupBox* resultGroup = new QGroupBox(QString::fromLocal8Bit("结果"));
    QVBoxLayout* resultLayout = new QVBoxLayout(resultGroup);

    matrixOutputText = new QTextEdit();
    matrixOutputText->setReadOnly(true);
    matrixOutputText->setStyleSheet("background-color: #f5f5f5; font-family: monospace;");
    matrixOutputText->setMinimumHeight(150);
    resultLayout->addWidget(matrixOutputText);
    mainLayout->addWidget(resultGroup);

    // 第五行：按钮
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(cancelBtn);
    mainLayout->addLayout(buttonLayout);

    // 连接信号槽
    connect(typeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
    connect(applyBtn, &QPushButton::clicked, this, &Matrix44OperationDialog::onApply);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    // 新增：连接点坐标变化的信号
    connect(pointXSpin, SIGNAL(valueChanged(double)), this, SLOT(onPointInputChanged()));
    connect(pointYSpin, SIGNAL(valueChanged(double)), this, SLOT(onPointInputChanged()));
    connect(pointZSpin, SIGNAL(valueChanged(double)), this, SLOT(onPointInputChanged()));

    drawer = new CoordinateSystemDrawer(m_context, m_v3dView);

    // 在世界的0点绘制一个参考基准三维坐标轴
    Matrix44 identityMatrix = Matrix44::IDENTITY;
    drawer->Draw("Origin", identityMatrix, 2, 5, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);

    // 初始化状态
    onTypeChanged(0);
}

Matrix44OperationDialog::~Matrix44OperationDialog() {}

void Matrix44OperationDialog::onApply()
{
    applyMatrixOperation();
}

void Matrix44OperationDialog::applyMatrixOperation()
{
    if (typeCombo->currentIndex() == 0) // 平移
    {
        double dx = xSpin->value();
        double dy = ySpin->value();
        double dz = zSpin->value();
        newMatrix = newMatrix * Matrix44::Translation(dx, dy, dz);

        updateMatrixDisplay(newMatrix);
        updatePointDisplay(newMatrix);  // 更新点显示
    }
    else if (typeCombo->currentIndex() == 1) // 旋转
    {
        double angle = angleSpin->value();
        double dx = xSpin->value();
        double dy = ySpin->value();
        double dz = zSpin->value();
        Vector3d axis(dx, dy, dz);
        if (axis == Vector3d::ZERO)
        {
            return;
        }
        newMatrix = newMatrix * Matrix44::Rotate(axis, angle);

        updateMatrixDisplay(newMatrix);
        updatePointDisplay(newMatrix);  // 更新点显示
    }
    else if (typeCombo->currentIndex() == 3 || typeCombo->currentIndex() == 4) // 左乘或右乘
    {
        updateMatrixDisplay(newMatrix);
        updatePointDisplay(newMatrix);  // 更新点显示
    }
}

void Matrix44OperationDialog::updateMatrixDisplay(const Matrix44& newMatrix)
{
    QString matrixText = QString::fromLocal8Bit("变换矩阵:\n");
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            matrixText += QString::number(newMatrix.m[i][j], 'f', 3) + " ";
        }
        matrixText += "\n";
    }

    // 添加点变换结果
    Vector3d originalPoint = getInputPoint();
    Vector3d transformedPoint = newMatrix * originalPoint;

    matrixText += QString::fromLocal8Bit("\n点变换结果:\n");
    matrixText += QString::fromLocal8Bit("原始点: (%1, %2, %3)\n")
        .arg(originalPoint.x).arg(originalPoint.y).arg(originalPoint.z);
    matrixText += QString::fromLocal8Bit("变换后: (%1, %2, %3)")
        .arg(transformedPoint.x).arg(transformedPoint.y).arg(transformedPoint.z);

    matrixOutputText->setText(matrixText);
}

void Matrix44OperationDialog::updatePointDisplay(const Matrix44& matrix)
{
    // 清除之前的点
    m_context->Erase(m_originalPoint, Standard_True);
    m_context->Erase(m_transformedPoint, Standard_True);

    // 获取输入点
    Vector3d originalPoint = getInputPoint();
    Vector3d transformedPoint = matrix * originalPoint;

    // 创建原始点（红色）
    gp_Pnt origPnt(originalPoint.x, originalPoint.y, originalPoint.z);
    m_originalPoint = new AIS_Point(new Geom_CartesianPoint(origPnt));
    m_originalPoint->SetColor(Quantity_NOC_RED);
    m_originalPoint->SetDisplayMode(1);  // 大尺寸显示
    m_context->Display(m_originalPoint, Standard_False);

    // 创建变换后的点（绿色）
    gp_Pnt transPnt(transformedPoint.x, transformedPoint.y, transformedPoint.z);
    m_transformedPoint = new AIS_Point(new Geom_CartesianPoint(transPnt));
    m_transformedPoint->SetColor(Quantity_NOC_GREEN);
    m_transformedPoint->SetDisplayMode(1);  // 大尺寸显示
    m_context->Display(m_transformedPoint, Standard_False);

    m_v3dView->Redraw();
}

Matrix44 Matrix44OperationDialog::getInputMatrix()
{
    Matrix44 inputMatrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            inputMatrix.SetValue(i, j, matrixInputs[i][j]->value());
        }
    }
    return inputMatrix;
}

Matrix44 Matrix44OperationDialog::getSecondInputMatrix()
{
    Matrix44 inputMatrix;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            inputMatrix.SetValue(i, j, matrixInputs2[i][j]->value());
        }
    }
    return inputMatrix;
}

Vector3d Matrix44OperationDialog::getInputPoint()
{
    return Vector3d(pointXSpin->value(), pointYSpin->value(), pointZSpin->value());
}

void Matrix44OperationDialog::onPointInputChanged()
{
    // 当点坐标变化时，重新应用当前操作以更新显示
    if (typeCombo->currentIndex() == 0 || typeCombo->currentIndex() == 1 ||
        typeCombo->currentIndex() == 3 || typeCombo->currentIndex() == 4)
    {
        applyMatrixOperation();
    }
}

void Matrix44OperationDialog::onTypeChanged(int idx)
{
    matrix = getInputMatrix();
    bool isTrans = (idx == 0);
    bool isRot = (idx == 1);
    bool isInv = (idx == 2);
    bool isLeft = (idx == 3);
    bool isRight = (idx == 4);
    bool isInvMult = (idx == 5);

    // 控制输入控件的可见性
    xSpin->setVisible(isTrans || isRot);
    ySpin->setVisible(isTrans || isRot);
    zSpin->setVisible(isTrans || isRot);
    angleSpin->setVisible(isRot);

    m_context->Erase(m_mat1Trihedron, Standard_True);

    if (isTrans || isRot)
    {
        newMatrix = getInputMatrix();
        m_mat1Trihedron = drawer->Draw("Original", newMatrix, 5, 25.0, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);
        updatePointDisplay(newMatrix);  // 显示原始点
    }
    else if (isInv)
    {
        newMatrix = matrix.Inverse();
        m_mat1Trihedron = drawer->Draw("Original", newMatrix, 5, 25.0, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);
        updateMatrixDisplay(newMatrix);
        updatePointDisplay(newMatrix);
    }
    else if (isLeft || isRight || isInvMult)
    {
        Matrix44 matrix2 = getSecondInputMatrix();
        if (isLeft)
        {
            newMatrix = matrix * matrix2;
            m_mat1Trihedron = drawer->Draw("Original", newMatrix, 5, 25.0, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);
            updateMatrixDisplay(newMatrix);
            updatePointDisplay(newMatrix);
        }
        else if (isRight)
        {
            newMatrix = matrix2 * matrix;
            m_mat1Trihedron = drawer->Draw("Original", newMatrix, 5, 25.0, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);
            updateMatrixDisplay(newMatrix);
            updatePointDisplay(newMatrix);
        }
        else if (isInvMult)
        {
            Matrix44 inverseMatrix = matrix2.Inverse();
            newMatrix = matrix * inverseMatrix;
            m_mat1Trihedron = drawer->Draw("Original", newMatrix, 5, 25.0, CoordinateSystemDrawer::ScaleMode::FromMatrix, true);
            updateMatrixDisplay(newMatrix);
            updatePointDisplay(newMatrix);
        }
    }
}
