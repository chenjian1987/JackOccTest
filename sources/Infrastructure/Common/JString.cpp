#include <sstream>
#include <iomanip>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "JString.h"
#include "StringUtils.h"

using namespace JackC;

//wstring::npos是wstirng的静态变量，表示一个无效的或者不存在的字符串位置或索引,这个值统筹用于查找某个字符的位置，当找不到时，则返回这个表示查找失败
const int JString::npos = (int)std::wstring::npos;
const JString JString::endl = L"\n\r";


JString::JString(){}
JString::JString(const std::wstring& strW):m_theString(strW){}
JString::JString(const wchar_t* pStrW):m_theString(pStrW){}

JString::JString(const std::string& str, bool isNative)
{
    if (isNative)
        m_theString = StringUtils::ToWStringByCodeType(str, StringUtils::CT_ANSI);
    else
        m_theString = StringUtils::ToWString(str);
}

JString::JString(const char* pStr, bool isNative)
{
    if (isNative)
    {
        m_theString = StringUtils::ToWStringByCodeType(pStr, StringUtils::CT_ANSI);
    }
    else
    {
        m_theString = StringUtils::ToWString(pStr);
    }
}

JString JString::operator + (const JString& _other) const
{
    JString rtStr;
    rtStr = *this;
    rtStr += _other;
    return rtStr;
}

JString& JString::operator += (const JString& _other)
{
    getStringFW() += _other.getString();
    return *this;
}

JString& JString::assign(const JString& _other)
{
    getStringFW().assign(_other.getString());
    return *this;
}

JString& JString::replace(int pos, int size, const JString& newStr)
{
    getStringFW().replace(pos, size, newStr.getString());
    return *this;
}

void JString::replaceSubString(const JString& oldSubString, const JString& newSubString)
{
    int pos = 0;
    int a = oldSubString.size();
    int b = newSubString.size();
    while ((pos = find(oldSubString, pos)) != JString::npos)
    {
        replace(pos, a, newSubString);
        pos += b;
    }
}

void JString::removeQuotationMark()
{
    while (!m_theString.empty() && m_theString.front() == L'\"')
    {
        m_theString = m_theString.substr(1, m_theString.length() - 1);
    }
    while (!m_theString.empty() && m_theString.back() == L'\"')
    {
        m_theString = m_theString.substr(0, m_theString.length() - 1);
    }
}

JString& JString::operator << (const JString& _other)
{
    m_theString += _other.m_theString;
    return *this;
}

JString& JString::operator << (int valInt)
{
    m_theString += StringUtils::ToWString(valInt);
    return *this;
}

JString& JString::operator << (long long valInt64)
{
    m_theString += StringUtils::ToWString(valInt64);
    return *this;
}

JString& JString::operator << (double valDouble)
{
    m_theString += StringUtils::ToWString(valDouble, 20);
    return *this;
}

JString JString::substr(int pos, int n) const
{
    return m_theString.substr(pos, n);
}
