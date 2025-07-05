#include <string>
#include <sstream>
#include <iomanip>
#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT StringUtils
    {
    public:
        enum EnCodeType
        {
            CT_ANSI = 0,
            CT_UTF8 = 1
        };

        template <typename T>
        inline static std::wstring ToWString(T t)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value,
                "only accept int"
                );
            std::wostringstream woss;
            woss << t;
            return woss.str();
        }

        template <typename T>
        inline static std::wstring ToString(T t)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value,
                "only accept int"
                );
            std::wostringstream woss;
            woss << t;
            return woss.str();
        }

        //精确到小数点precision位
        inline static std::wstring ToWString(double t, int precision = 20)
        {
            std::wostringstream woss;
            woss << std::fixed << std::setprecision(precision) << t;
            return woss.str();
        }
        //精确到小数点precision位
        inline static std::wstring ToWString(float t, int precision = 6)
        {
            std::wostringstream woss;
            woss << std::fixed << std::setprecision(precision) << t;
            return woss.str();
        }

        static std::wstring ToWString(bool value);

        template <typename T>
        inline static T ToNumber(const std::wstring& str, bool* pOk = nullptr)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value,
                "only accept int"
                );

            std::wistringstream wiss(TrimSpace(str));
            T result;
            wiss >> result;
            if (pOk)
            {
                *pOk = wiss.eof() && !wiss.bad() && !wiss.fail();   //eof正常结束，bad fail对付空字符串
            }
            return result;
        }
        template <typename T>
        inline static T ToNumber(const std::string& str, bool* pOk = nullptr)
        {
            static_assert(
                std::is_same<T, short>::value ||
                std::is_same<T, unsigned short>::value ||
                std::is_same<T, int>::value ||
                std::is_same<T, unsigned int>::value ||
                std::is_same<T, long>::value ||
                std::is_same<T, unsigned long>::value ||
                std::is_same<T, long long>::value ||
                std::is_same<T, unsigned long long>::value ||
                std::is_same<T, float>::value ||
                std::is_same<T, double>::value,
                "only accept int"
                );

            std::istringstream wiss(TrimSpace(str));
            T result;
            wiss >> result;
            if (pOk)
            {
                *pOk = wiss.eof() && !wiss.bad() && !wiss.fail();   //eof正常结束，bad fail对付空字符串
            }
            return result;
        }


        static bool ToBool(const std::string& str);
        static bool ToBool(const std::wstring& str);

        //use utf-8 by default
        static std::wstring ToWString(const std::string& str);
        //use utf-8 by default
        static std::wstring ToWString(const char* pStr);
        //下面的重载不加的话，clang会特化上面的模板
        static std::wstring ToWString(char* pStr);

        //use utf-8 by default
        static std::string ToString(const std::wstring& str);
        static std::string ToString(const wchar_t* pchar);
        static std::string ToString(wchar_t* pchar)
        {
            return ToString((const wchar_t*)pchar);
        }

        //如文件路径，命令行参数等和本地系统 编码相关，才建议使用
        static std::string  ToStringByCodeType(const std::wstring& src, EnCodeType codeType);
        static std::wstring ToWStringByCodeType(const std::string& src, EnCodeType codeType);

        static void replaceAll(std::wstring& inputStr, const std::wstring& oldValue, const std::wstring& newValue);
        //trim是否去除子串的首尾空白字符
        static std::vector<std::wstring> splitString(const std::wstring& strData, const std::wstring& seperator, bool trimSpace = false);
        static std::vector<std::string>  splitString(const std::string& strData, const std::string& seperator, bool trimSpace = false);

        //返回移除首尾空白字符的字符串
        static std::wstring TrimSpace(const std::wstring& str);
        static std::string  TrimSpace(const std::string& str);
        //返回移除首尾指定字符的字符串
        static std::wstring Trim(const std::wstring& str, std::function<bool(wchar_t)> func);
        static bool EndWith(const std::wstring& str, const std::wstring& endStr);
        static bool StartWith(const std::wstring& str, const std::wstring& endStr);

        static std::wstring CollapseToOneLine(const std::wstring& inputStr, const std::wstring& lineSpliter = L" | ");
        static std::wstring FormatLogParams(const std::wstring& inputStr);

        static std::wstring FormatWString(const std::wstring fmt_std, ...);

        template <class... Args>
        static std::wstring StrCat(Args&&... args)
        {
            std::wostringstream woss;
            StrCatImp(woss, std::forward<Args>(args)...);
            return woss.str();
        }

    private:
        static std::wostream& StrCatImp(std::wostream& out) {
            return out;
        }
        template <class First, class... Rest>
        static std::wostream& StrCatImp(std::wostream& out, First&& f,
            Rest&&... rest) {
            out << std::forward<First>(f);
            return StrCatImp(out, std::forward<Rest>(rest)...);
        }
    };


    //忽略大小写的字符串比较函数 StringCompareUtil::CompareInogreCase的语法糖
    int JACKC_COMMON_INTERFACE_EXPORT StrICmp(const char* pLHS, const char* pRHS);
    int JACKC_COMMON_INTERFACE_EXPORT StrICmp(const wchar_t* pLHS, const wchar_t* pRHS);
}
