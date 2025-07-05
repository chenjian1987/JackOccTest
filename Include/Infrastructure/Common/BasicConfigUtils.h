#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"


namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT BasicConfigUtils
    {
    private:
        static bool CompareConfigValue(const std::wstring& key, int target);
        static bool CompareConfigValue(const std::wstring& key, const std::wstring& target);
    public:
        static bool DebugMessageEnabled();
        static bool IsRunAsService();
        static bool LogEnabled();
        static bool DumpEnabled();
        static bool IsAppInHouse();
    };
}
