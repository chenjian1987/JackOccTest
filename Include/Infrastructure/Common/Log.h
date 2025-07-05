#pragma once
#include "JDevService.h"
#include "LogBase.h"

namespace JackC
{
    // 记录Dump信息
    class JACKC_COMMON_INTERFACE_EXPORT DumpLog : public LogBase
    {
    public:
        // 创建失败返回nullptr
        static OwnerPtr<DumpLog> CreateInstance(const std::wstring& logsFolder, const std::wstring& fileNameWithoutExt);

        // 输出text。simplified为true表示使用简化格式；默认为false
        void LogDumpLine(const std::wstring& text, bool simplified = false);
        void LogDumpLineToSpecialFile(const std::wstring& file, const std::wstring& text, bool simplified = false);
        virtual ~DumpLog(void);

    protected:
        DumpLog(const std::wstring& appConfigKey);
    };

    // 记录日志运行中Dump信息
    class JACKC_COMMON_INTERFACE_EXPORT JournalDumpLog : public DumpLog
    {
    public:
        // 创建失败返回nullptr
        static OwnerPtr<JournalDumpLog> CreateInstance(const std::wstring& logsFolder, const std::wstring& fileNameWithoutExt);

        virtual ~JournalDumpLog(void);

    protected:
        JournalDumpLog(const std::wstring& appConfigKey);
    };

    // 记录日志
    class JournalLog : public LogBase
    {
    public:
        // 创建失败返回nullptr
        static OwnerPtr<JournalLog> CreateInstance(const std::wstring& logsFolder, const std::wstring& fileNameWithoutExt);

        // 输出一行类型为logType的text内容
        void LogbmpLine(LogTypeOfLogManager logType, const std::wstring& text);

        virtual ~JournalLog(void);

    protected:
        JournalLog(const std::wstring& appConfigKey);
    };

    // 记录详细的日志
    class JACKC_COMMON_INTERFACE_EXPORT JournalFullLog : public JournalLog
    {
    public:
        // 创建失败返回nullptr
        static OwnerPtr<JournalFullLog> CreateInstance(const std::wstring& logsFolder, const std::wstring& fileNameWithoutExt);

        // 输出一行类型为logType的详细内容text
        void LogbmpFullLine(LogTypeOfLogManager logType, const std::wstring& text);

        virtual ~JournalFullLog(void);

    protected:
        JournalFullLog(const std::wstring& appConfigKey);
    };
}
