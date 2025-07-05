#pragma once

#include "ElementDefine.h"
#include "JDevService.h"



namespace JackC
{
    class ClassId;
    enum class OmPropertySchemaType :int8_t;

    /// <summary>
    /// ClassId相关辅助接口
    /// </summary>
    class JACKC_ELEMENT_EXPORT OmClassIdUtils
    {
    public:
        /// <summary>
        /// 普通值类型的ClassId
        /// </summary>
        static ClassId ms_valueClassId;
        /// <summary>
        /// ElementId类型的ClassId
        /// </summary>
        static ClassId ms_elementIdClassId;

        /// <summary>
        /// DbObjectSchematic类型的ClassId  
        /// </summary>
        static ClassId ms_schematicObjectClassId;

        /// <summary>
        /// DbObjectCloneable类型的ClassId 
        /// </summary>
        static ClassId ms_cloneableObjectClassId;

        /// <summary>
        /// IDbObject类型的ClassId  
        /// </summary>
        static ClassId ms_idbObjectClassId;

        /// <summary>
        /// OwnerPtr<DbObject>类型的ClassId   
        /// </summary>
        static ClassId ms_ownerPtrToDbObjectClassId;

        /// <summary>
        /// OwnerPtr<IDbObject>类型的ClassId   
        /// </summary>
        static ClassId ms_ownerPtrToIDbObjectClassId;

        /// <summary>
        /// 裸指针类型的ClassId  
        /// </summary>
        static ClassId ms_nativePointerClassId;

        /// <summary>
        /// 字符串类型的ClassId  
        /// </summary>
        static ClassId ms_wstringClassId;

        /// <summary>
        /// Vector容器类型的ClassId  
        /// </summary>
        static ClassId ms_vectorClassId;

        /// <summary>
        /// 集合set容器类型的ClassId 
        /// </summary>
        static ClassId ms_setClassId;

        /// <summary>
        /// Map容器类型的ClassId
        /// </summary>
        static ClassId ms_mapClassId;

        /// <summary>
        /// 无序Map类型的ClassId
        /// </summary>
        static ClassId ms_unorderedMapClassId;


    public:
        /// <summary>
        /// 通过目标属性类型枚举值获取对应的ClassId
        /// ClassId 和 OmPropertySchemaType
        ///   1.同一枚举对应的ClassId固定不变，如果成功识别枚举值则返回正常ClassId
        ///      否则返回的Id为无效的，可通过:ClassId::IsValid()判断
        ///   2.通过该方式获取的ClassId是Class Version不会改变，一直为1
        /// </summary>
        /// <param name="propertySchemaType"></param>
        /// <returns></returns>
        static ClassId GetClassIdOfPropertySchemaType(OmPropertySchemaType propertySchemaType);
    };


}