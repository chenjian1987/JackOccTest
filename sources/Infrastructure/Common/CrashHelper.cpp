#include "CrashHelper.h"
#include <sstream>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

CrashHelper* CrashHelper::GetInstance()
{
    static CrashHelper s_crashHelper;
    return &s_crashHelper;
}

void CrashHelper::Crash(const std::wstring& text, const std::wstring& fileName, int line, const std::wstring& function, const std::wstring& errorTips) const
{
    std::wostringstream stream;
    stream << L"File: " << fileName.c_str() << L"\n\rLine: " << line << L"\n\rFunction: " << function << L"\n\r\n\r";
    stream << L"Crash:" << text << L"\n\r\n\r";
    stream << L"CrashTips:" << errorTips;

#if defined(__clang__) &&defined(__linux__)
    // 暂不同时输出到Log
#else
    // 先输出到日志中
    //LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_CRASH, woss.str(), true);
#endif
    abort();
}