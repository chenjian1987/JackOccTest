#pragma once
#include "JDevService.h"
#include "LogBase.h"
#include "OwnerPtr.h"
#include "LogType.h"
#include "JCommonDefine.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace JackC
{
    class DumpLog;
    class JournalDumpLog;
    class JournalLog;
    class JournalFullLog;
    class ConsistencyDetecLog;  //consistency一致性

    class JACKC_COMMON_INTERFACE_EXPORT DumpIntentSetter
    {
    public:
        DumpIntentSetter(int intentDelta);
        virtual ~DumpIntentSetter();
    private:
        int m_intentDelta;
    };

    class JACKC_COMMON_INTERFACE_EXPORT LogIntentSetter
    {
    public:
        LogIntentSetter(int intentDelta);
        ~LogIntentSetter();
    private:
        int m_intentDelta;
    };


    class JACKC_COMMON_INTERFACE_EXPORT LogManager
    {
    private:
        static OwnerPtr<LogManager> m_opTheInstance;
        std::wstring m_logsFolder;
        std::wstring                      m_subFolderName;
        std::wstring                      m_fileNameWithoutExt;

        OwnerPtr<DumpLog>  m_opDumpLogInstance;
        OwnerPtr<JournalDumpLog>  m_opJournalDumpLogInstance;
        OwnerPtr<JournalLog>  m_opJournalLogInstance;
        OwnerPtr<JournalFullLog> m_opJournalFullLogInstance;
    private:
        LogManager(const std::wstring& subFolderName, const std::wstring& fileNamePrefix, bool numberAutomatically = false);
        bool ValidateLine(LogTypeOfLogManager lmlt, const LogBase* pLogBase, const std::wstring& text) const;

    public:
        virtual ~LogManager(void);
    public:
        //获取LogManager单例，如果不存在，则先创建.
        //subFolderName如果为空，文件夹格式为journal+自动编号
        //fileNamePrefix如果为空，则和文件夹名字一样
        //numberJSFile如果为true，则同名.js文件就会添加上自动编号，从XXX0001.js到 XXX9999.js
        static LogManager* CreateOrGetForWrite(const std::wstring& subFolderName = L"", const std::wstring& fileNamePrefix = L"", bool numberJSFile = false);
        //获取LogManager单例
        static LogManager* Get();
        //查询LogManager单例是否准备好
        static bool IsReady();


    public:
        void Restart();

        //输出到脚本日志
        void WriteLine(LogTypeOfLogManager lnlt, const std::wstring& text, bool copyToCompactedLog = false);
        //输出到dump文件
        void DumpLine(const std::wstring& text, bool simplified, bool inOneLine);
        void DumpLineToSpecialFile(const std::wstring& file, const std::wstring& text, bool simplified, bool isOneLine);

        // 输出到Dump文件（没有缩进）
        void DumpLineWithoutIntent(const std::wstring& text);

        // 输出到脚本日志专属Dump文件
        void JournalDumpLine(const std::wstring& text);

        // 获取Dump缩进大小
        int GetDumpIntentSize() const;

        // 设置Dump缩进大小
        void SetDumpIntentSize(int intentSize);

        // 获取Log缩进大小
        int GetLogIntentSize() const;

        // 设置Log缩进大小
        void SetLogIntentSize(int intentSize);

        // 获取Logs文件夹
        std::wstring GetLogsFolder() const;
        std::wstring GetBaseLogsDirPath() const;

        // 相对Log目录的绝对路径/相对路径转换
        std::wstring GetAbsolutePathForLog(const std::wstring& path) const;
        std::wstring GetRelativePathForLog(const std::wstring& fullPath) const;
    public:
        // 调试用，输出自身状态信息
        void Dump() const;

        // 获取日志
        const JournalFullLog* GetJournalFullLogInstance() const { return m_opJournalFullLogInstance.get(); }

        JournalFullLog* GetJournalFullLogInstance() { return m_opJournalFullLogInstance.get(); }
    };

}

#define FYI(text)  \
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(JackC::LogTypeOfLogManager::FYI,text,false);

#define FYI_IN_BOTH_FULL_AND_COMPACT(text) \
    JackC::LogManager::CreateOrGetForWrite()->WriteLine(JackC::LogTypeOfLogManager::FYI, text, true);

#define FYI_IF(condition, text) \
    if(condition) \
    { \
        FYI(text); \
    }

#define DUMP_TO_FILE(file,text) \
    JackC::LogManager::CreateOrGetForWrite(L"",file,false)->DumpLineToSpecialFile(file,text, false, false);

#define DUMP(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLine(text, false, false);

#define DUMP_IN_ONE_LINE(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLine(text, false, true);

#define DUMP_SIMPLIFIED(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLine(text, true, false);

#define DUMP_SIMPLIFIED_IN_ONE_LINE(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLine(text, true, true);

#define JOURNAL_DUMP(text) \
    JackC::LogManager::CreateOrGetForWrite()->JournalDumpLine(text);

#define DUMP_NO_INTENT(text) \
    JackC::LogManager::CreateOrGetForWrite()->DumpLineWithoutIntent(text);
//
#define DUMP_CONSISTENCY_RESULT(text) \
    JackC::LogManager::CreateOrGetForWrite()->ConsistencyDetectDumpLine(text);


#pragma warning(pop)