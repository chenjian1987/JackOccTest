#pragma once
//Qt
#include "qobject.h"
#include "qdebug.h"
#include "qregularexpression.h"
#include "qmatrix4x4.h"

//occ
#include "AIS_Shape.hxx"
#include "AIS_SequenceOfInteractive.hxx"
#include "AIS_Trihedron.hxx"
#include "AIS_InteractiveContext.hxx"
#include "BRep_Builder.hxx"
#include "BRepBuilderAPI_Sewing.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBndLib.hxx"
#include "BRepPrimAPI_MakeBox.hxx"
#include "BRepPrimAPI_MakeCylinder.hxx"
#include "Geom_Axis2Placement.hxx"
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"

//Assimp
#include "importer.hpp"
#include "postprocess.h"
#include "scene.h"
#include "ai_assert.h"
#include "vector3.h"
#include "Exporter.hpp"

using namespace Assimp;


enum class ModelType
{
    MODEL_COMPONT = 1,    //部件
    MODEL_COMP_GROUP = 2  //部件组
};


//AIS_Shape:可视化对象

//定义一个可视化对象，因为通过拓扑构造的对象需要 转化为可视化对象，这样Viewer才能显示此形体对象
class ModelShape :public AIS_Shape
{
    //此宏定义了DynamicType()方法，返回Handle实际引用类的Standard_Type单例的类型描述符句柄。类型描述符保存了类的名称及其父类的描述符
    DEFINE_STANDARD_RTTIEXT(ModelShape, AIS_Shape);

public:
    ModelShape(const TopoDS_Shape& shape = TopoDS_Shape());   //TopoDS_Shape拓扑对象基类

public:
    /// <summary>
    /// 设置模型名称
    /// </summary>
    /// <param name="name"></param>
    inline void setModelName(QString name)
    {
        m_modelName = name;
    }
    /// <summary>
    /// 获取模型名称
    /// </summary>
    /// <returns></returns>
    inline QString getModelName()
    {
        return m_modelName;
    }
    //设置模型类型
    inline void setModelType(ModelType type) { m_modelType = type; }
    inline ModelType getModelType() { return m_modelType; }


    /// <summary>
    /// 使用QMatrix4x4矩阵设置模型位置
    /// </summary>
    /// <param name="matrix"></param>
    void setPositionByMatrix(QMatrix4x4 matrix);
    /// <summary>
    /// 获取模型AABB包围盒
    /// </summary>
    /// <returns></returns>
    Bnd_Box getAABBbox();
    /// <summary>
    /// 处理所有Assimp节点
    /// </summary>
    /// <param name="node">节点</param>
    /// <param name="scene">场景</param>
    /// <param name="isRoot">是否为根节点</param>
    void processAssimpNodes(const aiNode* node, const aiScene* scene, bool isRoot);
    /// <summary>
    /// 模型材质转换，由Assimp转换为occ的表达方式
    /// </summary>
    /// <param name="material"></param>
    /// <returns></returns>
    Graphic3d_MaterialAspect materialTransfer(aiMaterial* material);
private:
    // 模型名称
    QString m_modelName;
    // 模型类型
    ModelType m_modelType;
};



DEFINE_STANDARD_HANDLE(ModelShape, AIS_Shape);
