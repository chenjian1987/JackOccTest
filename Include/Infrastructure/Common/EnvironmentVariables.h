#pragma once

#ifdef _WINDOWS
#include <Windows.h>
#endif

#include "JDefines.h"
#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
    namespace EnvironmentVariables
    {
        FORCE_INLINE std::wstring GetVariable(const std::wstring& name)
        {
            std::wstring res;
#ifdef _WINDOWS
            if (name.empty()) return res;

            //GetEnvironmentVariableW从调用该函数的进程的环境变量中返回指定变量名值的函数，参数1要获取值的变脸名字符串指针，参数2接手变量的字符串指针，参数3接收变量值的内存大小
            //   如果执行成功，表示写入字符串缓冲区的字符数量，但不包含\0字符
            DWORD dwRet = ::GetEnvironmentVariableW(name.c_str(), nullptr, 0);
            if (dwRet != 0)
            {
                LPTSTR pssOldVal = (LPTSTR)malloc(dwRet * sizeof(TCHAR));
                ::GetEnvironmentVariableW(name.c_str(), pssOldVal, dwRet);
                res = pssOldVal;
                free(pssOldVal);
            }
#endif
            return res;
        }

        FORCE_INLINE void SetVariable(const std::wstring& name, const std::wstring& value)
        {
            if (name.empty()) return;
#ifdef _WINDOWS
            SetEnvironmentVariableW(name.c_str(), value.c_str());
#endif
        }

        FORCE_INLINE std::wstring AddPath(const std::wstring& folderPath)
        {
            std::wstring resultPath = folderPath;

#ifdef _WINDOWS
            const std::wstring varName = L"Path";
            const std::wstring separator = L";";
            const std::wstring originalPath = GetVariable(varName);
            if (originalPath.size()>0)
            {
                resultPath += separator;
                resultPath += originalPath;
            }
#endif
            return resultPath;
        }
    }

    class JACKC_COMMON_INTERFACE_EXPORT  EnvironmentVariableSetter
    {
    public:
        EnvironmentVariableSetter(const std::wstring& name, const std::wstring& value):m_name(name)
        {
            if (m_name.empty()) return;

            m_originalValue = EnvironmentVariables::GetVariable(m_name);
            EnvironmentVariables::SetVariable(m_name.c_str(), value.c_str());
        }

        virtual ~EnvironmentVariableSetter()
        {
            if (m_name.empty()) return;
            EnvironmentVariables::SetVariable(m_name.c_str(), m_originalValue.c_str());
        }
    private:
        std::wstring m_name;
        std::wstring m_originalValue;
           
    };


}
