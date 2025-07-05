#include "IfcImportUtils.h"
#include <AIS_Shape.hxx>



TopoDS_Shape IfcImportUtils::ConvertCarveMeshToOCC(const std::shared_ptr<carve::mesh::MeshSet<3>>& meshset)
{
    if (!meshset) return TopoDS_Shape();

    const auto& verts = meshset->vertex_storage;
    std::vector<gp_Pnt> occ_points;
    for (const auto& vtx : verts)
        occ_points.emplace_back(vtx.v.x, vtx.v.y, vtx.v.z);

    std::vector<std::array<int, 3>> occ_tris;
    for (const auto& mesh : meshset->meshes) {
        if (!mesh) continue;
        for (const auto& face : mesh->faces) {
            if (!face) continue;
            if (face->nVertices() != 3) continue;
            std::vector<const carve::mesh::Vertex<3>*> fverts;
            auto edge = face->edge;
            for (size_t i = 0; i < 3; ++i) {
                fverts.push_back(edge->vert);
                edge = edge->next;
            }
            int id0 = static_cast<int>(fverts[0] - &verts[0]);
            int id1 = static_cast<int>(fverts[1] - &verts[0]);
            int id2 = static_cast<int>(fverts[2] - &verts[0]);
            occ_tris.push_back({ id0 + 1, id1 + 1, id2 + 1 });
        }
    }
    if (occ_points.empty() || occ_tris.empty()) return TopoDS_Shape();

    Handle(Poly_Triangulation) triangulation = new Poly_Triangulation((int)occ_points.size(), (int)occ_tris.size(), Standard_False);
    for (int i = 1; i <= occ_points.size(); ++i)
        triangulation->SetNode(i, occ_points[i - 1]);
    for (int t = 1; t <= occ_tris.size(); ++t)
        triangulation->SetTriangle(t, Poly_Triangle(occ_tris[t - 1][0], occ_tris[t - 1][1], occ_tris[t - 1][2]));

    TopoDS_Face face;
    BRep_Builder builder;
    builder.MakeFace(face);
    builder.UpdateFace(face, triangulation);

    return face;
}



void IfcImportUtils::DisplayAllMeshes(const std::shared_ptr<ItemShapeData>& item, Handle(AIS_InteractiveContext) m_context)
{
    if (!item) return;
    // 当前节点的所有 meshset
    for (const auto& meshset : item->m_meshsets)
    {
        if (!meshset) continue;
        TopoDS_Shape occ_shape = ConvertCarveMeshToOCC(meshset);
        if (!occ_shape.IsNull()) {
            Handle(AIS_Shape) aisShape = new AIS_Shape(occ_shape);
            m_context->Display(aisShape, Standard_True);
        }
    }
    // 递归所有子节点
    for (const auto& child : item->m_child_items)
    {
        DisplayAllMeshes(child, m_context);
    }
}