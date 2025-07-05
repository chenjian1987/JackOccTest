#pragma once

#include "JCommonDefine.h"
#include "JDevService.h"
#include "JTypeDefs.h"
#include "JNew.h"

namespace JackC
{
    //类ElementId用来表示Element在文档中的唯一标识.
    class JACKC_COMMON_INTERFACE_EXPORT ElementId
    {
    public:
        //构造函数
        FORCE_INLINE explicit ElementId(int64_t id = -1) : m_id(id) {}

        //析构函数
        FORCE_INLINE ~ElementId() {}

        //拷贝构造函数
        FORCE_INLINE ElementId(const ElementId& other) : m_id(other.m_id) {}

        //赋值重载函数
        FORCE_INLINE ElementId& operator=(const ElementId& other) { m_id = other.m_id; return *this; }
        FORCE_INLINE ElementId& operator=(const ElementId* other) { m_id = other->m_id; return *this; }

        //判断是否是有效的id
        FORCE_INLINE bool IsValid() const { return m_id >= 0; }

        //获取当前ElementId的数值表示
        FORCE_INLINE int64_t AsInt64() const { return m_id; }

        //重载==符，判断两个ElementId的值是否相同
        FORCE_INLINE bool operator==(const ElementId& elementId) const { return m_id == elementId.m_id; }

        //重载!=符，判断两个ElementId的值是否不同
        FORCE_INLINE bool operator!=(const ElementId& elementId)const { return m_id != elementId.m_id; }

        //重载>符，判断两个ElementId的大小关系
        FORCE_INLINE bool operator>(const ElementId& elementId)const { return m_id > elementId.m_id; }

        //重载<符，判断两个ElementId的大小关系
        FORCE_INLINE bool operator<(const ElementId& elementId)const { return m_id < elementId.m_id; }

    public:

        //创建一个无效的ElementId
        static ElementId CreateInvalidId() { return InvalidID; }

        //通过指定值创建一个ElementId
        static ElementId Create(int64_t id) { return ElementId(id); }

        //获取当前ElementId的数值
        int64_t GetId() const { return m_id; }

        //判断当前ElementId是否是有效的
        bool GetIsValidId() const { return IsValid(); }

        //判断两个ElementId的值是否相同
        bool HasSameValue(const ElementId& elementId) const { return (m_id == elementId.m_id); }

    public:
        static const ElementId InvalidID;
    private:
        int64_t m_id;
    };

    IMMORTAL_TYPE_NAME(ElementId);

    //重载<<运算符, 宽字符
    FORCE_INLINE std::wostream& operator<<(std::wostream& os, ElementId elemId)
    {
        return os << elemId.AsInt64();
    }

    //重载<<运算符
    FORCE_INLINE std::ostream& operator<<(std::ostream& os, ElementId elemId)
    {
        return os << elemId.AsInt64();
    }

    //ElementId的hash值计算
    struct ElementIdHash
    {
        std::size_t operator()(const ElementId& key) const
        {
            return std::hash<int64_t>()(key.AsInt64());
        }
    };

    using ElementIdArray = std::vector<ElementId>;
    using ElementIdSet = std::set<ElementId>;
    using UnorderedElementIdSet = std::unordered_set<ElementId, ElementIdHash>;
    using CategoryId = ElementId;
    using FamilyId = ElementId;
}
