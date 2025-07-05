#pragma once

namespace JackC
{
    //日志文件中每一行日志的类型。 如果添加新类型，需要同步对应字符串到s_logMgrLogTypeName
    enum class LogTypeOfLogManager
    {
        INVALID = 0,
        DEBUG_CRASH = 1,
        DEBUG_ERROR = 2,
        DEBUG_WARN = 3,
        DEBUG_INFO = 4,
        DIAGNOSE = 5,
        FYI = 6,
        JOURNAL = 7,
        JOURNAL_WARN = 8,
        JOURNAL_ERROR = 9,
        CONSISTENCY = 10,
        NUM_OF_TYPES = 11,
    };
}
