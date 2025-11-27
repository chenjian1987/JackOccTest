#include "CoordinateSystemDrawer.h"

#include <gp_Ax2.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Graphic3d_TransformPers.hxx>
#include <Quantity_NameOfColor.hxx>



CoordinateSystemDrawer::CoordinateSystemDrawer(const Handle(AIS_InteractiveContext)& context, const Handle(V3d_View)& view) :m_context(context), m_view(view)
{

}
CoordinateSystemDrawer::~CoordinateSystemDrawer()
{
    ClearAll();
}

void CoordinateSystemDrawer::SetShowLabels(bool show) { m_showLabels = show; }
void CoordinateSystemDrawer::SetShowArrows(bool show) { m_showArrows = show; }


Handle(AIS_Trihedron) CoordinateSystemDrawer::Draw(const std::string& id, const Matrix44& mat, double axisWidth ,double axisLength , ScaleMode mode, bool persistent)
{
    if (m_context.IsNull()) return nullptr;
    gp_Pnt origin;
    gp_Dir xdir,ydir, zdir;
    double x, y, z;

    Extract(mat, origin, xdir, ydir, zdir, x, y, z);
    double axisLengths[3];
    if (mode == ScaleMode::FromMatrix)
    {
        axisLengths[0] = axisLength * x;
        axisLengths[1] = axisLength * y;
        axisLengths[2] = axisLength * z;
    }
    else
    {
        axisLengths[0] = axisLength ;
        axisLengths[1] = axisLength ;
        axisLengths[2] = axisLength ;
    }
    const double axisWidths[3] = { axisWidth,axisWidth,axisWidth };
    gp_Ax2 axis2(origin, zdir, xdir);

    Handle(Geom_Axis2Placement) placement = new Geom_Axis2Placement(axis2);
    Handle(AIS_Trihedron) tri = new AIS_Trihedron(placement);
    Handle(Prs3d_Drawer) drawer = Draw(axisLength, axisWidths,m_showLabels, m_showArrows, xdir, ydir, zdir, axisLengths);
    tri->SetAttributes(drawer);

    if (!id.empty())
    {
        auto iter = m_items.find(id);
        if (iter != m_items.end())
        {
            m_context->Erase(iter->second.tri, Standard_True);
            m_items.erase(iter);
        }
    }
    m_context->Display(tri, Standard_True);
    Entry ent;
    ent.tri = tri;
    ent.drawer = drawer;
    ent.persistent = persistent;
    m_items[id] = ent;

    return tri;
}

Handle(Prs3d_Drawer) CoordinateSystemDrawer::Draw(double axisLen, const double axisWidths[3], bool drawLabels, bool drawArraows,
    const gp_Dir& xdir, const gp_Dir& ydir, const gp_Dir& zdir, const double axisLens[3])
{
    Handle(Prs3d_DatumAspect) datum = new Prs3d_DatumAspect();
    datum->SetDrawLabels(drawLabels ? Standard_True : Standard_False);
    datum->SetDrawArrows(drawArraows? Standard_True : Standard_False);

    datum->SetAxisLength(axisLens[0],axisLens[1], axisLens[2]);

    //x
    datum->LineAspect(Prs3d_DP_XAxis)->SetWidth(axisWidths[0]);
    datum->LineAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
    datum->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);
    datum->TextAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED);

    //y
    datum->LineAspect(Prs3d_DP_YAxis)->SetWidth(axisWidths[1]);
    datum->LineAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datum->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);
    datum->TextAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN);

    //z
    datum->LineAspect(Prs3d_DP_ZAxis)->SetWidth(axisWidths[2]);
    datum->LineAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);
    datum->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);
    datum->TextAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE1);

    Handle(Prs3d_Drawer) drawer = new Prs3d_Drawer();
    drawer->SetDatumAspect(datum);
    return drawer;
}



void CoordinateSystemDrawer::Extract(const Matrix44& mat, gp_Pnt& origin, gp_Dir& xdir, gp_Dir& ydir, gp_Dir& zdir, double& x, double& y, double& z)
{
    origin = gp_Pnt(mat.m[0][3], mat.m[1][3], mat.m[2][3]);

    gp_Vec xVec(mat.m[0][0], mat.m[1][0], mat.m[2][0]);
    gp_Vec yVec(mat.m[0][1], mat.m[1][1], mat.m[2][1]);
    gp_Vec zVec(mat.m[0][2], mat.m[1][2], mat.m[2][2]);

    x=xVec.Magnitude();
    y = yVec.Magnitude();
    z = zVec.Magnitude();

    if (x > 1e-12) xdir = gp_Dir(xVec.Normalized());
    else xdir = gp_Dir(1, 0, 0);

    if (y > 1e-12) ydir = gp_Dir(yVec.Normalized());
    else ydir = gp_Dir(0, 1, 0);

    if (z > 1e-12) zdir = gp_Dir(zVec.Normalized());
    else zdir = gp_Dir(0, 0, 1);
}


void CoordinateSystemDrawer::Update(const std::string& id, const Matrix44& mat,double axisWidth ,double axisLength, ScaleMode mode)
{
    Draw(id, mat, axisWidth,axisLength, mode, false);
}

void CoordinateSystemDrawer::Erase(const std::string& id)
{
    auto iter = m_items.find(id);
    if (iter != m_items.end())
    {
        if (!m_context.IsNull() && !iter->second.tri.IsNull())
        {
            m_context->Erase(iter->second.tri, Standard_True);
            if (!m_view.IsNull())
                m_view->Redraw();
        }
        m_items.erase(iter);
    }
}

void CoordinateSystemDrawer::ClearAll()
{
    for (auto& item : m_items)
    {
        if (!m_context.IsNull() && !item.second.tri.IsNull())
        {
            m_context->Erase(item.second.tri, Standard_True);
        }
    }
    m_items.clear();
    if (!m_view.IsNull())
        m_view->Redraw();
}