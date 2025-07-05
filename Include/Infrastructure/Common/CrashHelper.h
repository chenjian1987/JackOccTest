#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
    //±¿¿£π§æﬂ¿‡
    class  JACKC_COMMON_INTERFACE_EXPORT  CrashHelper
    {
    private:
        CrashHelper() {}
        virtual ~CrashHelper() {}

    public:
        static CrashHelper* GetInstance();

        void Crash(const std::wstring& expression, const std::wstring& fileName, int line, const std::wstring& funtion, const std::wstring& crashTips) const;
    };
}