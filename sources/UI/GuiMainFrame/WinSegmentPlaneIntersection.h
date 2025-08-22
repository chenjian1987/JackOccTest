#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QList>
#include <functional>
#include <cmath>

#include "GeometryUtilityOccDefine.h"
#include "GeomLineSegmentRelationUtility.h"
#include "OwnerPtr.h"

using namespace JackC;

class WinSegmentPlaneIntersection : public QDialog
{
    Q_OBJECT
public:
    explicit WinSegmentPlaneIntersection(Handle(AIS_InteractiveContext) ctx,
        Handle(V3d_Viewer) viewer,
        Handle(V3d_View)    view,
        QWidget* parent = nullptr, std::function<void(const QString&)> outputFunc = {});

private slots:
    void onDraw();      // 绘制线段与平面
    void onCompute();   // 计算交点
    void onClear();     // 清空场景与结果

private:
    // UI 构建
    void setupUI();

    // 解析输入
    bool parsePoint(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez, gp_Pnt& P, const QString& tip);
    bool parseVector(QLineEdit* ex, QLineEdit* ey, QLineEdit* ez, gp_Vec& V, const QString& tip);
    bool parseDouble(QLineEdit* e, double& v, const QString& tip);

    // 绘制辅助
    void drawSegment(const gp_Pnt& p1, const gp_Pnt& p2);
    void drawPlane(const gp_Pln& pln, double halfSize);
    void drawPoint(const gp_Pnt& p, Quantity_NameOfColor color, const QString& text);
    void drawDash(const gp_Pnt& a, const gp_Pnt& b, const Quantity_Color& col, Standard_Real width = 2.0);

private:
    // 场景
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_Viewer)             m_viewer;
    Handle(V3d_View)               m_view;

    // 缓存可视对象，便于清理
    QList<Handle(AIS_InteractiveObject)> m_objects;

    // ===== UI 控件 =====
    // 线段 P1 / P2
    QLineEdit* m_p1x = nullptr, * m_p1y = nullptr, * m_p1z = nullptr;
    QLineEdit* m_p2x = nullptr, * m_p2y = nullptr, * m_p2z = nullptr;

    // 平面：点 P0 与法向 N
    QLineEdit* m_p0x = nullptr, * m_p0y = nullptr, * m_p0z = nullptr;
    QLineEdit* m_nx = nullptr, * m_ny = nullptr, * m_nz = nullptr;

    // 平面显示半尺寸
    QLineEdit* m_halfSize = nullptr;

    // 算法选择
    QComboBox* m_combo = nullptr;

    // 按钮与结果
    QPushButton* m_btnDraw = nullptr, * m_btnCompute = nullptr, * m_btnClear = nullptr;
    QLabel* m_result = nullptr;

    OwnerPtr< GeomLineSegmentRelationUtility> m_opUti;
};

