#pragma once
#include "ElementDefine.h"
#include "JQuickCast.h"
#include "JTypeDefs.h"

#pragma warning(push)
#pragma warning(disable:4100)

namespace JackC
{
    class OmClassSchema;
    class ISerialierBehavior;
    class ICloneBehavior;

    /// <summary>
    /// Db数据对象基类， 不需要直接继承，通过继承DbObjectSchematic DbObjectCloneable DbElement 等它的派生类来使用
    /// </summary>
    class JACKC_ELEMENT_EXPORT DbObject
    {
        DEFINE_CAST_BASE(DbObject);
    public:
        explicit DbObject() {};
        virtual ~DbObject() {};

        /// <summary>
        /// 获取该DbObject的最顶层所有者， 顶层所有者通常是Element
        /// 如果该DbObject作为Element的成员，派生类一定要实现该方法，返回正确指针， 否则数据发生变化时 框架无法通知Element
        /// </summary>
        /// <returns></returns>
        virtual DbObject* GetTopOwnerObject() { return nullptr; }

    public:
        /// <summary>
        /// 获取类schema的指针
        /// </summary>
        /// <returns></returns>
        virtual const OmClassSchema* GetClassSchema() const { return nullptr; }
        /// <summary>
        /// 对象的部分数据不完善， 还用对应的DbObject表示，但是此字段返回false
        /// </summary>
        /// <returns></returns>
        virtual bool IsComplete() const { return true; }
        /// <summary>
        /// 设置是否 是一个完整的对象
        /// </summary>
        /// <param name="isComplete"></param>
        virtual void SetIsComplete(bool isComplete){}
        /// <summary>
        /// 获取到的序列化信息，传入值首先被清空
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        virtual bool GetContaineeData(std::vector<Byte>& bytes) const { return false; }
    };
}
#pragma warning(pop)