#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT JString
    {
    private:
        std::wstring m_theString;
    public:
        JString();
        JString(const wchar_t* pStrW);
        JString(const std::wstring& strW);

        //isNative只有和系统编码相关的转码操作才可使用
        JString(const char* pStr, bool isNative = true);
        JString(const std::string& str, bool isNative = true);

        //不同字符串的输出接口
    public:
        const std::wstring& getString() const { return m_theString; }
        std::wstring& getStringFW() { return m_theString; }
        const wchar_t* c_str() const { return getString().c_str(); }

        //各种操作符重载
    public:
        JString operator+(const JString& other) const;
        JString& operator+=(const JString& other);
        bool operator<(const JString& other) const
        {
            return getString() < other.getString();
        }
        bool operator==(const JString& other) const
        {
            return getString() == other.getString();
        }
        bool operator!=(const JString& other) const
        {
            return getString() != other.getString();
        }
        JString& operator << (const JString& valString);
        JString& operator << (int valInt);
        JString& operator << (long long valInt64);
        JString& operator << (double valDouble);

        //与std::wstring 对应的各种字符串的操作
    public:
        static const int npos;
        static const JString endl;
        bool empty() const { return getString().empty(); }
        int size() const { return (int)getString().size(); }
        int find(const JString& other, int pos) const { return (int)getString().find(other.getString(), pos); }
        JString substr(int pos, int n) const;

        JString& assign(const JString& other);
        JString& replace(int pos, int size, const JString& newStr);
        void clear() { getStringFW().clear(); }

    public:
        void replaceSubString(const JString& oldSubStr, const JString& newSubStr);
        void removeQuotationMark();

    };
}

#pragma warning(pop)