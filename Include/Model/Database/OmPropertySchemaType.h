#pragma once
#include <cstdint>

namespace JackC
{
    enum class OmPropertySchemaType :int8_t
    {
        Invalid=0,                 ///< 非法类型
        Value,                     ///< 可以直接内存拷贝的类型
        ElementId,                 ///< ElementId
        WString,                   ///< std::wstring
        SchematicObject,           ///< * 继承于DbObjectSchematic的类型
        CloneableObject,           ///< * 继承于DbObjectCloneable的类型
        IDbObject,                 ///< * 继承于IDbObject的类型
        NativePointer,             ///< 裸指针
        OwnerPtrToDbObject,        ///< * 指向Dbobject派生类的OwnerPtr
        OwnerPtrToIDbObject,       ///< * 指向IDbObject派生类的OwnerPtr
        Vector,                    ///< * DbVectorContainer
        Set,                       ///< * DbSetContainer
        Map,                       ///< * DbMapContainer
        UnorderedMap               ///< * DbUnorderedContainer

    };

    /// <summary>
    /// 属性行为类型
    /// </summary>
    enum class EnumPropertyBehaviourType :int32_t
    {
        Transing=0x000,
        SupportCopy=0x001,
        SupportTransaction=0x010,
        SupportArchive=0x100,
        SupportAll=0x111
    };
}