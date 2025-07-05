#pragma once 
#include "JGuid.h"
#include "JCommonDefine.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT ClassId
    {
    private:
        JGuid m_guid;
    public:
        ClassId() noexcept :m_guid(JGuid::GetInvalid())   //noexcept屏蔽异常
        {}
        ~ClassId();

        explicit ClassId(const JGuid& guid) :m_guid(guid) {}
        // 接受格式为：%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串，中间允许有空格。空串或者不符合格式的产生Invalid()的ClassId
        //explicit去掉参数的隐式转换，函数的传参 类型必须和形参一致
        explicit ClassId(const std::wstring& str);

        bool operator==(const ClassId& another) const;
        bool operator!=(const ClassId& another) const;
        bool operator>(const ClassId& another) const;
        bool operator<(const ClassId& another) const;

        bool IsValid() const;
        //返回格式为%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串表示
        std::wstring AsWString() const;
        // 接受格式为：%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X 的字符串，中间允许有空格。空串或者不符合格式的产生Invalid()的ClassId
        void SetByWString(const std::wstring& wstr);

        const Guid GetGuid() const
        {
            return m_guid.ToGuid();
        }
        const JGuid& GetCubeGuid() const
        {
            return m_guid;
        }
        void SetGuid(const JGuid& guid)
        {
            m_guid = guid;
        }

        const std::wstring GetGuidWString()
        {
            return m_guid.ToWString();
        }

    };

    struct JACKC_COMMON_INTERFACE_EXPORT ClassIdHasher
    {
        std::size_t operator() (const ClassId& key) const;
    };
}
