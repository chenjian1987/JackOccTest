#pragma once
#include <map>
#include <AIS_Trihedron.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include "GuiMainFrameDefine.h"
#include "Matrix44.h"


class JACKC_GUI_MAIN_FRAME_EXPORT CoordinateSystemDrawer
{
public:
    enum class ScaleMode
    {
        /// <summary>
        /// 使用统一的轴长
        /// </summary>
        Uniform,
        /// <summary>
        /// 使用矩阵向量的长度作为轴长
        /// </summary>
        FromMatrix
    };

    CoordinateSystemDrawer(const Handle(AIS_InteractiveContext)& context, const Handle(V3d_View)& view);
    ~CoordinateSystemDrawer();

    Handle(AIS_Trihedron) Draw(const std::string& id, const Matrix44& mat, double axisWidth = 2 , double axisLength = 100, ScaleMode mode = ScaleMode::Uniform, bool persistent = false);

public:
    Handle(Prs3d_Drawer) Draw(double axisLen, const double axisWidths[3], bool drawLabels, bool drawArraows,
        const gp_Dir& xdir, const gp_Dir& ydir, const gp_Dir& zdir, const double axisLens[3]);

    void Extract(const Matrix44& mat, gp_Pnt& origin, gp_Dir& xdir, gp_Dir& ydir, gp_Dir& zdir, double& x, double& y, double& z);

    // 更新已存在 id 的坐标系（若不存在则绘制）
    void Update(const std::string& id, const Matrix44& mat, double axisWidth = 2, double axisLength = 100.0,ScaleMode mode = ScaleMode::Uniform);

    // 擦除指定 id 的坐标系
    void Erase(const std::string& id);

    // 擦除并删除所有坐标系
    void ClearAll();

    // 设置轴文字/箭头是否显示
    void SetShowLabels(bool show);
    void SetShowArrows(bool show);

private:
    struct Entry
    {
        Handle(AIS_Trihedron) tri;
        Handle(Prs3d_Drawer) drawer;

        bool persistent;
    };

    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;

    std::map<std::string, Entry> m_items;
    bool m_showLabels = true;
    bool m_showArrows = true;

};