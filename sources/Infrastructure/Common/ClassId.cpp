#include <regex>
#include <ostream>
#include <string>
#include "ClassId.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

namespace
{
    void RegulateWString(std::wstring& outputStr, const std::wstring& input)
    {
        //Regulate规定，控制  reserve预留空间
        outputStr.reserve(input.size() + 4);
        for (const auto& item : input)   //for(auto item:vt) 将每个vt的值拷贝给item，这个过程必然会浪费内存，所有要使用引用，就存在拷贝构造了，浪费内存。 所以用这个方式， const来限制不允许修改， 并加上引用
        {
            if (!::iswspace(item))  //iswspace给定的宽字符是否为宽空白字符
                outputStr.push_back(toupper(item));
        }
    }

    // 是否符合{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}格式
    bool IsValidRegulatedWString(const std::wstring& regulatedWString)
    {
        const wchar_t* pRegex = LR"(^\{[0-9A-F]{8}-([0-9A-F]{4}-){2}([0-9A-F]{2}){2}-([0-9A-F]{2}){6}\}$)";
        const std::wregex hexPattern(pRegex);
        return std::regex_match(regulatedWString, hexPattern);
    }
}

ClassId::ClassId(const std::wstring& wstr):m_guid(JGuid::GetInvalid())
{
    if (!wstr.empty())
    {
        std::wstring r_wstring;
        RegulateWString(r_wstring, wstr);
        m_guid = JGuid::JGuid(r_wstring);
    }
}
ClassId::~ClassId()
{
    int i = 0;
}

bool ClassId::operator==(const ClassId& another) const
{
    if (this == &another) return true;
    return JGuid::Equal(m_guid, another.m_guid);
}

bool ClassId::operator!=(const ClassId& another) const
{
    if (this == &another)
    {
        return false;
    }
    return !this->operator==(another);
}

bool ClassId::operator>(const ClassId& another) const
{
    return this->m_guid > another.m_guid;
}

bool ClassId::operator<(const ClassId& another) const
{
    return this->m_guid < another.m_guid;
}

bool ClassId::IsValid() const
{
    return m_guid.IsValid();
}

std::wstring ClassId::AsWString() const
{
    return m_guid.ToWString();
}

void ClassId::SetByWString(const std::wstring& wstr)
{
    m_guid = JGuid(wstr);
}

std::size_t ClassIdHasher::operator()(const ClassId& key) const
{
    return key.GetCubeGuid().Hash();
}
