#pragma once
#include <AIS_InteractiveContext.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS_Face.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pnt.hxx>
#include <ifcpp/geometry/GeometryConverter.h>
#include <IfcProject.h>
#include <ifcpp/reader/ReaderSTEP.h>
#include <GeometrySettings.h>
#include <IncludeCarveHeaders.h>
#include <GeomDebugDump.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/StatusCallback.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/geometry/GeometrySettings.h>
#include <ifcpp/geometry/SolidModelConverter.h>
#include <ifcpp/geometry/FaceConverter.h>

#include <ifcpp/IFC4X3/include/IfcFacetedBrep.h>
#include <ifcpp/IFC4X3/include/IfcExtrudedAreaSolid.h>
#include <ifcpp/IFC4X3/include/IfcSolidModel.h>



class IfcImportUtils
{
public:
    static TopoDS_Shape ConvertCarveMeshToOCC(const std::shared_ptr<carve::mesh::MeshSet<3>>& meshset);

    static void DisplayAllMeshes(const std::shared_ptr<ItemShapeData>& item, Handle(AIS_InteractiveContext) m_context);

};