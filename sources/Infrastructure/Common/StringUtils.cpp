#include <codecvt>
#include <locale>
#include <Windows.h>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "JAssert.h"
#include "StringUtils.h"
#include <cassert>

using namespace JackC;


std::string StringUtils::ToString(const std::wstring& src)
{
    return ToStringByCodeType(src, CT_UTF8);
}
std::string StringUtils::ToString(const wchar_t* pStr)
{
    const std::wstring str(pStr);
    return ToString(str);
}
std::wstring StringUtils::ToWString(const std::string& src)
{
    return ToWStringByCodeType(src, CT_UTF8);
}
std::wstring StringUtils::ToWString(const char* pSrc)
{
    const std::string src(pSrc);
    return ToWString(src);
}
std::wstring StringUtils::ToWString(char* pSrc)
{
    return ToWString((const char*)pSrc);
}
std::wstring StringUtils::ToWString(bool value)
{
    return (value ? L"true" : L"false");
}

std::string StringUtils::ToStringByCodeType(const std::wstring& src, EnCodeType codeType)
{
    int codePage = -1;
    if (codeType == CT_ANSI)
    {
        codePage = CP_ACP;
    }
    else if (codeType == CT_UTF8)
    {
        codePage = CP_UTF8;
    }
    else
    {
        //JackC_ASSERT(false && L"Unsupported EnCodeType of StringUtil");
        return std::string();
    }
    //WideCharToMultiByte映射一个unicode字符串到一个多字节字符串  --Unicode转ANSI(GB2312))
    // 参数1：UINT 指定执行转换的代码页，这个惨呼可以为系统已经安装或有效的任何代码页给定的值，最常用的是CP_ACP和CP_UTF8,前者将宽字符转为ANSI，后面转换为UTF8
    //        CP_ACP ANSI代码页   CP_MACCP Macintosh代码页   CP_OEMCP OEM代码页   CP_SYMBOL 符号代码页 CP_UTF7  CP_UTF8
    // 参数2：指定如何处理没有转换的字符，可默认为0
    // 参数3：需要转换的宽字符串
    // 参数4：待展缓宽字符串的长度，-1表示转换到字符串的结尾
    // 参数5：输出缓冲区大小，如果为0，参数4被忽略
    // 参数6：指向字符的指针
    const int bytesNeeded = WideCharToMultiByte(codePage, 0, src.data(), -1, NULL, 0, NULL, NULL);
    //JackC_ASSERT(bytesNeeded > 0);

    std::vector<char> buf(bytesNeeded);
    const int bytesWritten = WideCharToMultiByte(codePage, 0, src.data(), -1, buf.data(), bytesNeeded, NULL, NULL);
    if (bytesWritten != 0)
    {
        return buf.data();
    }
    else
    {
        //JackC_ASSERT(false && L"Error occurred in WideCharToMultiByte()");
        assert(false && L"Error occurred in WideCharToMultiByte()");
        return std::string();
    }
}

std::wstring StringUtils::ToWStringByCodeType(const std::string& src, EnCodeType codeType)
{
#ifdef _WINDOWS
    int codePage = -1;
    if (codeType == CT_ANSI)
    {
        codePage = CP_ACP;
    }
    else if (codeType == CT_UTF8)
    {
        codePage = CP_UTF8;
    }
    else
    {
        //JackC_ASSERT(false && L"Unsupported EnCodeType of StringUtil");
        return std::wstring();
    }

    const int charactersNeeded = MultiByteToWideChar(codePage, 0, src.data(), -1, NULL, 0);
    //JackC_ASSERT(charactersNeeded > 0);

    std::vector<wchar_t> buf(charactersNeeded);
    const int charactersWritten = MultiByteToWideChar(codePage, 0, src.data(), -1, buf.data(), charactersNeeded);
    if (charactersWritten != 0)
    {
        return buf.data();
    }
    else
    {
        //JackC_ASSERT(false && L"Error occurred in MultiByteToWideChar()");
        return std::wstring();
    }
#else
    // Better way is to use std::wstring/std::string other than JackCString.
    // Since JackCString used everywhere, we force using UTF8 code convertor to avoid assert.
    //if (codeType == UTF8)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
        return conv.from_bytes(src);
    }
    //else
    //{
        //JackC_ASSERT(false && L"Unsupported EnCodeType of StringUtil");
        //return std::wstring();
    //}
#endif
}


void StringUtils::replaceAll(std::wstring& inputStr, const std::wstring& oldValue, const std::wstring& newValue)
{
    for (std::wstring::size_type pos(0); pos != std::wstring::npos; pos += newValue.length())
    {
        if ((pos = inputStr.find(oldValue, pos)) != std::wstring::npos)
        {
            inputStr.replace(pos, oldValue.length(), newValue);
        }
        else break;
    }
}
std::wstring StringUtils::FormatLogParams(const std::wstring& inputStr)
{
    std::wstring result = inputStr;
    replaceAll(result, L"\\", L"\\\\");
    replaceAll(result, L"\r", L"\\r");
    replaceAll(result, L"\n", L"\\n");
    replaceAll(result, L"\"", L"\\\"");
    return result;
}
std::wstring StringUtils::CollapseToOneLine(const std::wstring& inputStr, const std::wstring& lineSpliter)
{
    std::wstring result = inputStr;
    replaceAll(result, L"\n\r", lineSpliter);
    replaceAll(result, L"\r\n", lineSpliter);
    replaceAll(result, L"\r", lineSpliter);
    replaceAll(result, L"\n", lineSpliter);
    return result;
}

namespace
{
    template<class T>
    std::vector<T> SplitStringImpl(const T& strData, const T& seperator, bool trimSpace)
    {
        std::vector<T> results;
        if (seperator.empty())
        {
            results.push_back(strData);
            return results;
        }
        size_t start = 0;
        size_t end;
        while ((end = strData.find(seperator, start)) != T::npos)
        {
            T subStr = strData.substr(start, end - start);
            if (trimSpace)
            {
                subStr = StringUtils::TrimSpace(subStr);
            }
            results.push_back(subStr);
            start = end + seperator.size();
        }
        T subStr = strData.substr(start);
        if (trimSpace)
        {
            subStr = StringUtils::TrimSpace(subStr);
        }
        results.push_back(subStr);
        return results;
    }

    template<class StrT, class CharT>
    StrT TrimImpl(const StrT& str, std::function<bool(CharT)> func)
    {
        int start = 0;
        int end = (int)str.length() - 1;
        while (start < (int)str.length() && func(str[start]))
        {
            ++start;
        }
        while (end > start && func(str[end]))
        {
            --end;
        }
        if (end >= start)
        {
            return str.substr(start, end - start + 1);
        }
        return StrT();
    }
}

std::vector<std::wstring> StringUtils::splitString(const std::wstring& strData, const std::wstring& seperator, bool trimSpace)
{
    return SplitStringImpl<std::wstring>(strData, seperator, trimSpace);
}
std::vector<std::string> StringUtils::splitString(const std::string& strData, const std::string& seperator, bool trimSpace)
{
    return SplitStringImpl<std::string>(strData, seperator, trimSpace);
}

bool StringUtils::ToBool(const std::string& value)
{
    if (value == "TRUE" || value == "True" || value == "true" || value == "1")
        return true;
    if (value == "FALSE" || value == "False" || value == "false" || value == "0")
        return false;

    return false;
}
bool StringUtils::ToBool(const std::wstring& value)
{
    if (value == L"TRUE" || value == L"True" || value == L"true" || value == L"1")
        return true;
    if (value == L"FALSE" || value == L"False" || value == L"false" || value == L"0")
        return false;

    return false;
}

std::wstring StringUtils::TrimSpace(const std::wstring& str)
{
    return TrimImpl<std::wstring, wchar_t>(str, [](wchar_t ch) -> bool {
        std::locale loc;
        return std::isspace(ch, loc);
        });
}
std::string StringUtils::TrimSpace(const std::string& str)
{
    return TrimImpl<std::string, char>(str, [](wchar_t ch) -> bool {
        std::locale loc;
        return std::isspace(ch, loc);
        });
}
std::wstring StringUtils::Trim(const std::wstring& str, std::function<bool(wchar_t)> func)
{
    return TrimImpl<std::wstring, wchar_t>(str, func);
}

bool StringUtils::EndWith(const std::wstring& str, const std::wstring& endStr)
{
    int dis = (int)str.size() - (int)endStr.size();
    if (dis >= 0)
    {
        int i;
        for (i = 0; i < (int)endStr.size(); ++i)
        {
            if (endStr[i] != str[dis + i])
                break;
        }
        return (i == endStr.size());
    }
    return false;
}
bool StringUtils::StartWith(const std::wstring& str, const std::wstring& startStr)
{
    if (str.size() < startStr.size())
    {
        return false;
    }

    int i = 0;
    while (i < (int)startStr.size() && str[i] == startStr[i])
    {
        ++i;
    }

    return i == (int)startStr.size();
}


template <typename T>
struct CaseConverter
{
    inline static T ToLower(T t)
    {
        STATIC_ASSERT_FALSE("T should be only char or wchar_t");
    }
    inline static T ToUpper(T t)
    {
        STATIC_ASSERT_FALSE("T should be only char or wchar_t");
    }
};

template <>
struct CaseConverter<char>
{
    inline static char ToLower(char t)
    {
        return tolower(t);
    }
    inline static char ToUpper(char t)
    {
        return toupper(t);
    }
};

template <>
struct CaseConverter<wchar_t>
{
    inline static wchar_t ToLower(wchar_t t)
    {
        return towlower(t);
    }
    inline static wchar_t ToUpper(wchar_t t)
    {
        return towupper(t);
    }
};

//字符串比较工具类，解决跨平台忽略大小写的字符串的比较问题
template <typename T>
class StringCompareUtil
{
public:
    //字符串比较，不区分大小写
    //当JHS<RHS ，返回-1   =时 为0   >时为1
    inline static int CompareInogreCase(const T* pLHS, const T* pRHS)
    {
        if (pRHS == pLHS) return 0;

        size_t idx = 0;
        while (*(pLHS + idx) != 0 && *(pRHS + idx) != 0)
        {
            const T lC = CaseConverter<T>::ToLower(*(pLHS + idx));
            const T rC = CaseConverter<T>::ToLower(*(pRHS + idx));

            if (lC < rC) return -1;
            else if (lC > rC) return 1;
            ++idx;
        }
        if (*(pLHS + idx) == 0 && *(pRHS + idx) != 0)
        {
            return -1;
        }
        else if (*(pLHS + idx) != 0 && *(pRHS + idx) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};

int StrICmp(const wchar_t* pLHS, const wchar_t* pRHS)
{
    return StringCompareUtil<wchar_t>::CompareInogreCase(pLHS, pRHS);
}

int StrICmp(const char* pLHS, const char* pRHS)
{
    return StringCompareUtil<char>::CompareInogreCase(pLHS, pRHS);
}

std::wstring StringUtils::FormatWString(const std::wstring fmt_str, ...)
{
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    std::wstring str;
    std::vector<wchar_t> formatted;
    va_list ap;
    while (1) {
        formatted.resize(n); /* Wrap the plain char array into the unique_ptr */
        wcscpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vswprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return std::wstring(&formatted[0]);
}