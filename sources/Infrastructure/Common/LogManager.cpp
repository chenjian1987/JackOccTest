#include "LogManager.h"
#include "Log.h"
#include "JNew.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include <typeinfo>
#include "FileSystem.h"
#include "FilePath.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

OwnerPtr<LogManager> LogManager::m_opTheInstance;




//////////////////////////////////////////////////////////////////////////
LogManager::LogManager(const std::wstring& subFolderName, const std::wstring& fileNamePrefix, bool numberJSFile)
{
    std::wstring appLogsFolderPath = LogBase::GetAppLogsFolderPath();

    // 如果不存在, 创建Logs文件夹
    if (!FileInfo(appLogsFolderPath).Exists())
    {
        FileSystem::CreateDir(appLogsFolderPath);
    }

    m_subFolderName = subFolderName;
    if (m_subFolderName.empty())
    {
        m_logsFolder = LogBase::FindAvailablePath(LogBase::GetAppLogsFolderPath(), L"journal", L"");
    }
    else
    {
        m_logsFolder = LogBase::GetAppLogsFolderPath() + FileSystem::Separator() + m_subFolderName;
    }

    std::wstring fileNameWithoutExt;
    if (fileNamePrefix.empty())
    {
        fileNameWithoutExt = m_logsFolder.substr(m_logsFolder.find_last_of(FileSystem::Separator()) + 1);
    }
    else
    {
        fileNameWithoutExt = fileNamePrefix;
    }

    if (numberJSFile)
    {
        const std::wstring fullFilePath = LogBase::FindAvailablePath(m_logsFolder, fileNameWithoutExt, L".js");
        const std::wstring fileName = fullFilePath.substr(fullFilePath.find_last_of(FileSystem::Separator()) + 1);
        fileNameWithoutExt = fileName.substr(0, fileName.find_last_of(L"."));
    }

    m_opJournalLogInstance = JournalLog::CreateInstance(m_logsFolder, fileNameWithoutExt);
    m_opJournalFullLogInstance = JournalFullLog::CreateInstance(m_logsFolder, fileNameWithoutExt);
}

LogManager::~LogManager()
{

}





LogManager* LogManager::Get()
{
    //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(ms_upTheInstance, L"ms_upTheInstance不应该为空");
    return m_opTheInstance.get();
}
std::wstring LogManager::GetLogsFolder() const
{
    return m_logsFolder;
}
std::wstring LogManager::GetBaseLogsDirPath() const
{
    std::wstring baseLogsDir = LogBase::GetAppLogsFolderPath() + FileSystem::Separator();
    //DBG_WARN_AND_RETURN_UNLESS(m_logsFolder.find(baseLogsDir) == 0, L"", L"Log目录的父目录不对");
    return baseLogsDir;
}
std::wstring LogManager::GetAbsolutePathForLog(const std::wstring& path) const
{
    if (FilePath::IsRelativePath(path))
    {
        // 回放时，需要把文件存入相对于Log目录的地方
        return FilePath::GetAbsolutePath(path, GetLogsFolder());
    }
    return path;
}
std::wstring LogManager::GetRelativePathForLog(const std::wstring& fullPath) const
{
    std::wstring logFolderPath = GetLogsFolder();
    if (FilePath::IsOnSameDrive(fullPath, logFolderPath))
    {
        return FilePath::GetRelativePath(fullPath, logFolderPath);
    }
    else
    {
        return fullPath;
    }
}




LogManager* LogManager::CreateOrGetForWrite(const std::wstring& subFolderName, const std::wstring& fileNamePrefix, bool numberFile)
{
    if (!LogManager::IsReady())
    {
        m_opTheInstance = NEW_AS_OWNER_PTR(LogManager, subFolderName, fileNamePrefix, numberFile);
    }
    //JackC_ASSERT(LogManager::IsReady() && L"LogManager should be ready now.");
    return m_opTheInstance.get();
}
void LogManager::WriteLine(LogTypeOfLogManager lmlt, const std::wstring& text, bool copyToCompactedLog)
{
    if (!ValidateLine(lmlt, m_opJournalLogInstance.get(), text))    return;
    if (copyToCompactedLog && !ValidateLine(lmlt, m_opJournalFullLogInstance.get(), text))
    {
        return;
    }

    std::wstring line = StringUtils::CollapseToOneLine(text);
    m_opJournalFullLogInstance->LogbmpFullLine(lmlt, line);

    if (copyToCompactedLog)
    {
        m_opJournalLogInstance->LogbmpLine(lmlt, line);
    }

    // 对DBG_X类和诊断类的信息，同时输出到调试窗口

#ifdef _WINDOWS
    if ((lmlt >= LogTypeOfLogManager::DEBUG_CRASH && lmlt <= LogTypeOfLogManager::DEBUG_INFO) || lmlt == LogTypeOfLogManager::DIAGNOSE)
    {
        OutputDebugString(text.c_str());
        OutputDebugString(L"\n");
    }
#endif
}
void LogManager::DumpLine(const std::wstring& text, bool simplified, bool inOneLine)
{
    if (!ValidateLine(LogTypeOfLogManager::FYI, m_opDumpLogInstance.get(), text))
    {
        return;
    }

    if (inOneLine)
    {
        m_opDumpLogInstance->LogDumpLine(StringUtils::CollapseToOneLine(text), simplified);
    }
    else
    {
        m_opDumpLogInstance->LogDumpLine(text, simplified);
    }

    // 同时输出到调试窗口
#ifdef _WINDOWS
    OutputDebugString(text.c_str());
    OutputDebugString(L"\n");
#endif
}
void LogManager::DumpLineToSpecialFile(const std::wstring& file, const std::wstring& text, bool simplified, bool inOneLine)
{
    if (!ValidateLine(LogTypeOfLogManager::FYI, m_opDumpLogInstance.get(), text))
    {
        return;
    }

    if (inOneLine)
    {
        m_opDumpLogInstance->LogDumpLineToSpecialFile(file, StringUtils::CollapseToOneLine(text), simplified);
    }
    else
    {
        m_opDumpLogInstance->LogDumpLineToSpecialFile(file, text, simplified);
    }

    // 同时输出到调试窗口
#ifdef _WINDOWS
    OutputDebugString(text.c_str());
    OutputDebugString(L"\n");
#endif
}



bool LogManager::IsReady()
{
    return m_opTheInstance != nullptr;
}
bool LogManager::ValidateLine(LogTypeOfLogManager lmlt, const LogBase* pLogBase, const std::wstring& text) const
{
    if ((uint32_t)lmlt <= 0 || (uint32_t)lmlt >= (uint32_t)LogTypeOfLogManager::NUM_OF_TYPES)
    {
        //DBG_WARN(L"错误的日志记录类型");
        return false;
    }
    if (nullptr == pLogBase || text.empty())   return false;
    if (!pLogBase->IsEnabled())                return false;
    if (!pLogBase->IsReady())                  return false;
    if (pLogBase->GetIntentSize() >= 100)
    {
        //DBG_WARN(L"日志嵌套层数过大");
        return false;
    }
    return true;
}



void LogManager::JournalDumpLine(const std::wstring& text)
{
    if (!ValidateLine(LogTypeOfLogManager::FYI, m_opJournalDumpLogInstance.get(), text))
    {
        return;
    }

    std::wstring line = StringUtils::CollapseToOneLine(text);

    m_opJournalDumpLogInstance->LogDumpLine(line);

    // 同时输出到调试窗口
#ifdef _WINDOWS
    OutputDebugString(text.c_str());
    OutputDebugString(L"\n");
#endif
}
void LogManager::DumpLineWithoutIntent(const std::wstring& text)
{
    DumpIntentSetter dumpIntentSetter(-m_opJournalDumpLogInstance->GetIntentSize());
    DumpLine(text, false, false);
}
void LogManager::Dump() const
{
    std::wostringstream oss;
    oss << L"Start dumping " << typeid(*this).name() << L"...";
    DUMP(oss.str());
    {
        DumpIntentSetter dis(2);

        oss.str(L"");
        oss << L"Current Log Intent: " << m_opJournalLogInstance->GetIntentSize();
        DUMP(oss.str());

        oss.str(L"");
        oss << L"Current Dump Intent: " << m_opJournalDumpLogInstance->GetIntentSize();
        DUMP(oss.str());
    }
}
int LogManager::GetDumpIntentSize() const
{
    return m_opDumpLogInstance->GetIntentSize();
}
void LogManager::SetDumpIntentSize(int intentSize)
{
    m_opDumpLogInstance->SetIntentSize(intentSize);
}
int LogManager::GetLogIntentSize() const
{
    return m_opJournalLogInstance->GetIntentSize();
}
void LogManager::SetLogIntentSize(int intentSize)
{
    m_opJournalLogInstance->SetIntentSize(intentSize);
}


void LogManager::Restart()
{
    if (m_subFolderName.empty())
    {
        m_logsFolder = LogBase::FindAvailablePath(LogBase::GetAppLogsFolderPath(), L"journal", L"");
    }
    else
    {
        m_logsFolder = LogBase::GetAppLogsFolderPath() + FileSystem::Separator() + m_subFolderName;
    }
    m_opJournalLogInstance = JournalLog::CreateInstance(m_logsFolder, m_fileNameWithoutExt);
    m_opJournalFullLogInstance = JournalFullLog::CreateInstance(m_logsFolder, m_fileNameWithoutExt);
}





//////////////////////////////////////////////////////////////////////////
DumpIntentSetter::DumpIntentSetter(int intentDelta): m_intentDelta(intentDelta)
{
    LogManager::CreateOrGetForWrite()->SetDumpIntentSize(LogManager::Get()->GetDumpIntentSize() + m_intentDelta);
}
DumpIntentSetter::~DumpIntentSetter()
{
    LogManager::CreateOrGetForWrite()->SetDumpIntentSize(LogManager::Get()->GetDumpIntentSize() - m_intentDelta);
}
LogIntentSetter::LogIntentSetter(int intentDelta)
    : m_intentDelta(intentDelta)
{
    LogManager::CreateOrGetForWrite()->SetLogIntentSize(LogManager::Get()->GetLogIntentSize() + m_intentDelta);
}
LogIntentSetter::~LogIntentSetter()
{
    LogManager::CreateOrGetForWrite()->SetLogIntentSize(LogManager::Get()->GetLogIntentSize() - m_intentDelta);
}
