#include "JAssert.h"
#include "IConfigRecord.h"
#include <sstream>
#include "LogManager.h"

using namespace JackC;

bool JackC::IsAssertEnabled()
{
    //const IConfigRecord* pEnableDebugMessageConfigRecord = IBehaviorConfig::GetConfigRecord(BehaviorConfigRecordNames::EnableDebugMessage);
    //if (pEnableDebugMessageConfigRecord)
    //{
    //    return pEnableDebugMessageConfigRecord->IsInternalIdentifyCodeValue();
    //}  DumpAssertToLog  IsAssertEnabled  DumpDbgWarnToLog
    return true;
}

void JackC::DumpAssertToLog(const std::string& expression,const std::string& file, int line, const std::string& function)
{
    std::wostringstream woss;
    woss << L"Assert: " << expression.c_str()
        << L", File: " << file.c_str()
        << L", Line: " << line
        << L", Function: " << function.c_str()
        << L".";
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_ERROR, woss.str(), true);
}

void JackC::DumpDbgWarnToLog(const std::wstring& message, const std::wstring& file, int line)
{
    std::wostringstream woss;
    woss << L"Warn: " << message << L", File: " << file << L", Line: " << line << L".";
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_WARN, woss.str(), true);
}
