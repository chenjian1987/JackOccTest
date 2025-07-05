#pragma once

#include <fstream>
#include <tuple>
#include <map>
#include "JCommonDefine.h"
#include "OwnerPtr.h"

#pragma warning (push)
#pragma warning (disable:4251)

namespace JackC
{
    enum class LogTypeOfLogManager;

    class JACKC_COMMON_INTERFACE_EXPORT  LogBase
    {
    private:
        std::wstring m_appConfigKey;
        int m_intentSize;
        bool m_isReady;
        std::wstring m_folderPath;
        std::wstring m_fileName;
        std::wstring m_fileExt;

        std::fstream m_fileStream;
        std::map<std::tuple<std::wstring, std::wstring, std::wstring>, OwnerPtr<std::fstream>> m_extraOutputPath;


    public:
        //初始化，需要提供文件夹，文件名（不带编号部分），文件扩展名（带'.'的后缀，例如".log"）参数
        void Init(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt);

        void AddExtraOutputPath(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt);
        void DeleteExtraOutputPath(const std::wstring& folderPath, const std::wstring& preferredFileName, const std::wstring& fileExt);

        // 查询是否准备好
        bool IsReady() const;
        // 查询是否启用
        bool IsEnabled() const;
        // 获取缩进
        int GetIntentSize() const;
        // 设置缩进
        void SetIntentSize(int intentSize);
        // 获取文件夹
        const std::wstring& GetFolderPath() const;
        // 获取文件名（不带编号）
        const std::wstring& GetFileName() const;
        // 获取文件拓展名
        const std::wstring& GetFileExt() const;
        // 获取应用程序设定的Log文件夹
        static std::wstring GetAppLogsFolderPath();
        // 根据文件夹、文件名（不含编号）、拓展名（带.，例如".log"），查找可用的全路径名
        static std::wstring FindAvailablePath(const std::wstring& folder, const std::wstring& prefix, const std::wstring& ext);

    protected:
        LogBase(const std::wstring& appConfigKey);

        void LogLine(bool withTimestamp, bool commentTimestamp, bool withLogType, bool commentLogType, LogTypeOfLogManager logType, const std::wstring& text);
        void LogLine(const std::wstring& file, bool withTimestamp, bool commentTimestamp, bool withLogType, bool commentLogType, LogTypeOfLogManager logType, const std::wstring& text);

        virtual ~LogBase() = 0;

        static std::wstring MakeTimeStamp();

        //日志的注释类型
        enum class CommentType :uint8_t
        {
            None = 0,              ///<没有
            SlashAsterisk = 1,     ///<斜杠和星号 /* */
            DoubleSlash = 2,       ///<双斜杠    //
            CommentTypeCount     ///<个数
        };

        static std::wstring Comment(const std::wstring& text, CommentType commentType);
    };
}

#pragma warning(pop)