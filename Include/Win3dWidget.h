#pragma once


#include "ModelShape.h"
#include <QWidget>
#include <QApplication>
#include <QMouseEvent>


/// <summary>
/// 3D显示窗口
/// </summary>
class Win3dWidget :public QWidget
{
    Q_OBJECT
public:
    Win3dWidget(QWidget* parent = nullptr);

    //! 获取三维环境交互对象
    Handle(AIS_InteractiveContext) getContext() { return m_context; }
    //! 获取三维显示界面
    Handle(V3d_View)  getView() { return m_view; }

    //创建矩形体
    void makeCube(Standard_Real dx = 1.0, Standard_Real dy = 1.0, Standard_Real dz = 1.0);
    //创建圆柱
    void makeCylinder(Standard_Real r = 0.5, Standard_Real h = 2.0);
    //创建球体
    void makeSphere(Standard_Real r = 1);
    //创建圆锥
    void makeCone(Standard_Real r1 = 1.0, Standard_Real r2 = 1.0, Standard_Real h = 2.0);
    //创建圆环
    void makeTorus(Standard_Real r1 = 2.0, Standard_Real r2 = 0.5);

protected:
    //覆写绘图事件
    void paintEvent(QPaintEvent*);
    //覆写窗口尺寸变化事件
    void resizeEvent(QResizeEvent*);
    //返回窗口的绘制引擎
    QPaintEngine* paintEngine() const;

    void mousePressEvent(QMouseEvent* event);
    //!覆写鼠标按键释放事件
    void mouseReleaseEvent(QMouseEvent* event);
    //!覆写鼠标移动事件
    void mouseMoveEvent(QMouseEvent* event);
    //!覆写鼠标滚轮事件
    void wheelEvent(QWheelEvent* event);

private:
    /// <summary>
    /// 初始化交互环境
    /// </summary>
    void initContext();

private:
    //交互式上下文能够管理一个或多个查看器(Viewer)中的图形行为和交互对象的选择
    Handle(AIS_InteractiveContext) m_context;
    //定义视图类型对象的上的服务
    Handle(V3d_Viewer) m_viewer;
    //创建一个视图
    Handle(V3d_View) m_view;
    //创建3d接口定义图形驱动程序
    Handle(Graphic3d_GraphicDriver) m_graphicDriver;

    Standard_Integer m_x_max;    //!记录鼠标平移坐标X
    Standard_Integer m_y_max;    //!记录鼠标平移坐标Y
};