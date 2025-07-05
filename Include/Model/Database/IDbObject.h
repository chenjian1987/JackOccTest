#pragma once
#include "ElementDefine.h"
#include "IObject.h"
#include "ClassId.h"

namespace JackC
{
    class ISerializeBehavior;
    class ICloneBehavior;
    class ClassId;

    class JACKC_ELEMENT_EXPORT IDbObject :public IObject
    {
        DEFINE_CAST_DERIVED(IDbObject, IObject);
    public:
        /// <summary>
        /// 获取class的唯一标识符
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        virtual Guid GetClassGuid(void) const { return JGuid::Generate(); }
        /// <summary>
        /// 获取序列化接口
        /// </summary>
        /// <param name=""></param>
        /// <returns></returns>
        virtual ISerializeBehavior* GetSerializeBehavior(void) const { return nullptr; }
        /// <summary>
        /// 获取克隆接口
        /// </summary>
        /// <returns></returns>
        virtual ICloneBehavior* GetCloneBehavior() const { return nullptr; }
    public:
        static ClassId GetClassId(void) { return ClassId(); }
        virtual ~IDbObject(void){}
    };
}