#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT ShellUtils
    {
    public:
        static void ExecuteShellCmd(const std::wstring& shellExecuteFilePath, const std::wstring& shellExecuteParameterString);
    };
}
