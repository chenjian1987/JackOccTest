#include "Log.h"
#include "IBehaviorConfig.h"
#include "JNew.h"
#include "FileSystem.h"
#include "LogType.h"

using namespace JackC;
DumpLog::DumpLog(const std::wstring& appConfigKey)
    : LogBase(appConfigKey)
{

}

DumpLog::~DumpLog()
{
}

void DumpLog::LogDumpLine(const std::wstring& text, bool simplified)
{
    if (simplified)
    {
        LogBase::LogLine(false, false, false, false, LogTypeOfLogManager::FYI, text);
    }
    else
    {
        LogBase::LogLine(true, false, true, false, LogTypeOfLogManager::FYI, text);
    }
}

void DumpLog::LogDumpLineToSpecialFile(const std::wstring& file, const std::wstring& text, bool simplified)
{
    if (simplified)
    {
        LogBase::LogLine(file, false, false, false, false, LogTypeOfLogManager::FYI, text);
    }
    else
    {
        LogBase::LogLine(file, true, false, true, false, LogTypeOfLogManager::FYI, text);
    }
}

//////////////////////////////////////////////////////////////////////////
JournalDumpLog::JournalDumpLog(const std::wstring& appConfigKey)
    : DumpLog(appConfigKey)
{

}

JournalDumpLog::~JournalDumpLog()
{
}

//////////////////////////////////////////////////////////////////////////
JournalLog::JournalLog(const std::wstring& appConfigKey) : LogBase(appConfigKey)
{

}

JournalLog::~JournalLog(void)
{
}

void JournalLog::LogbmpLine(LogTypeOfLogManager logType, const std::wstring& text)
{
    if (logType == LogTypeOfLogManager::JOURNAL)
    {
        LogBase::LogLine(false, false, false, true, logType, text);
    }
    else
    {
        LogBase::LogLine(false, false, true, true, logType, text);
    }
}

//////////////////////////////////////////////////////////////////////////
JournalFullLog::JournalFullLog(const std::wstring& appConfigKey)
    : JournalLog(appConfigKey)
{

}

JournalFullLog::~JournalFullLog()
{
}

void JournalFullLog::LogbmpFullLine(LogTypeOfLogManager logType, const std::wstring& text)
{
    if (logType == LogTypeOfLogManager::JOURNAL)
    {
        LogBase::LogLine(true, true, false, true, logType, text);
    }
    else
    {
        LogBase::LogLine(true, true, true, true, logType, text);
    }
}

OwnerPtr<DumpLog> DumpLog::CreateInstance(const std::wstring& logsFolder, const std::wstring& journalFileName)
{
    std::wstring::size_type pos = logsFolder.find_last_of(FileSystem::Separator());
    if (std::wstring::npos == pos)
    {
        return nullptr;
    }

    OwnerPtr<DumpLog> opLog = NEW_AS_OWNER_PTR(DumpLog, BehaviorConfigRecordNames::EnableDump);
    opLog->Init(logsFolder, std::wstring(L"dump_") + journalFileName, L".txt");
    return opLog;
}

OwnerPtr<JournalDumpLog> JournalDumpLog::CreateInstance(const std::wstring& logsFolder, const std::wstring& journalFileName)
{
    std::wstring::size_type pos = logsFolder.find_last_of(FileSystem::Separator());
    if (std::wstring::npos == pos)
    {
        return nullptr;
    }

    OwnerPtr<JournalDumpLog> opLog = NEW_AS_OWNER_PTR(JournalDumpLog, BehaviorConfigRecordNames::EnableDump);
    opLog->Init(logsFolder, std::wstring(L"dump_journal_") + journalFileName, L".txt");
    return opLog;
}

OwnerPtr<JournalLog> JournalLog::CreateInstance(const std::wstring& logsFolder, const std::wstring& journalFileName)
{
    std::wstring::size_type pos = logsFolder.find_last_of(FileSystem::Separator());
    if (std::wstring::npos == pos)
    {
        return nullptr;
    }

    OwnerPtr<JournalLog> opLog = NEW_AS_OWNER_PTR(JournalLog, BehaviorConfigRecordNames::EnableLog);
    opLog->Init(logsFolder, journalFileName, L".js");
    return opLog;
}

OwnerPtr<JournalFullLog> JournalFullLog::CreateInstance(const std::wstring& logsFolder, const std::wstring& journalFileName)
{
    std::wstring::size_type pos = logsFolder.find_last_of(FileSystem::Separator());
    if (std::wstring::npos == pos)
    {
        return nullptr;
    }

    OwnerPtr<JournalFullLog> opLog = NEW_AS_OWNER_PTR(JournalFullLog, BehaviorConfigRecordNames::EnableLog);
    opLog->Init(logsFolder, journalFileName + std::wstring(L"_full"), L".js");
    return opLog;
}
