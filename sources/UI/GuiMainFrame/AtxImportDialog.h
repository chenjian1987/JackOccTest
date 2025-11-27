#pragma once

#include <QDialog>

class QLineEdit;
class QPushButton;
class HomePageActionFun;

class AtxImportDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @param actionFun  指向 HomePageActionFun 的指针，用于执行 ImportIfc()
     * @param parent     父窗口
     */
    explicit AtxImportDialog(HomePageActionFun* actionFun,
        QWidget* parent = nullptr);

private slots:
    void onBrowse();
    void onImport();

private:
    void setupUi();

    HomePageActionFun* m_actionFun;
    QLineEdit* m_fileEdit;
    QPushButton* m_browseBtn;
    QPushButton* m_importBtn;
    QPushButton* m_cancelBtn;
};
