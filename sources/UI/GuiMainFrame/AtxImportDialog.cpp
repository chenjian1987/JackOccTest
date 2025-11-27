#include "AtxImportDialog.h"
#include "HomePageActionFun.h"

#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>



AtxImportDialog::AtxImportDialog(HomePageActionFun* actionFun,
    QWidget* parent)
    : QDialog(parent),
    m_actionFun(actionFun),
    m_fileEdit(new QLineEdit(this)),
    m_browseBtn(new QPushButton(QString::fromStdWString(L"浏览…"), this)),
    m_importBtn(new QPushButton(QString::fromStdWString(L"导入"), this)),
    m_cancelBtn(new QPushButton(QString::fromStdWString(L"取消"), this))
{
    setupUi();

    // 按钮信号
    connect(m_browseBtn, &QPushButton::clicked, this, &AtxImportDialog::onBrowse);
    connect(m_importBtn, &QPushButton::clicked, this, &AtxImportDialog::onImport);
    connect(m_cancelBtn, &QPushButton::clicked, this, &AtxImportDialog::reject);
}

void AtxImportDialog::setupUi()
{
    // 文件选择行
    auto fileLayout = new QHBoxLayout;
    fileLayout->addWidget(new QLabel(QString::fromStdWString(L"ATX 文件："), this));
    fileLayout->addWidget(m_fileEdit);
    fileLayout->addWidget(m_browseBtn);

    // 按钮行
    auto btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(m_importBtn);
    btnLayout->addWidget(m_cancelBtn);

    // 主布局
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);
    setWindowTitle(QString::fromStdWString(L"导入 ATX 文件"));
    resize(500, 120);
}

void AtxImportDialog::onBrowse()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        QString::fromStdWString(L"选择 ATX 文件"),
        QString(),
        QString::fromStdWString(L"ATX 文件 (*.atx *.ATX)")
    );
    if (!file.isEmpty())
        m_fileEdit->setText(file);
}

void AtxImportDialog::onImport()
{
    const QString path = m_fileEdit->text().trimmed();
    if (path.isEmpty()) {
        QMessageBox::warning(this, QString::fromStdWString(L"警告"), QString::fromStdWString(L"请先选择一个 ATX 文件"));
        return;
    }

    // 调用 HomePageActionFun 执行导入并渲染
    if (m_actionFun) {
        m_actionFun->ImportAtx(path);
        accept();
    }
    else {
        QMessageBox::critical(this, QString::fromStdWString(L"错误"), QString::fromStdWString(L"内部未初始化导入函数"));
        reject();
    }
}
