#pragma once

#include "JDefines.h"
#include "FilePath.h"
#include "JCommonDefine.h"

namespace JackC
{
    //用于程序相关的文件或文件夹的处理
    class JACKC_COMMON_INTERFACE_EXPORT FilePathManager
    {
        CANNOT_COPY_OR_ASSIGN(FilePathManager);

    public:
        static FilePathManager* Get();
        virtual ~FilePathManager(void);

        void Init();

        const FilePath& GetResourcePath();

        //不允许随便设置
        const FilePath& GetPublicConfigFilePath();
        const FilePath& GetBehaviorConfigFilePath();

        // 必须在外部程序启动时设置相应BehaviorConfig文件路径
        void SetBehaviorConfigFilePath(const std::wstring& behaviorConfigFilePath);

        const FilePath& GetLogsFolderPath();
        void SetLogsFolderPath(const std::wstring& logsFolderPath);

        const FilePath& GetTemplateFolderPath();
        void SetTemplateFolderPath(const std::wstring& templateFolderPath);

        const FilePath& GetGitRevisionFilePath();
        void SetGitRevisionFilePath(const std::wstring& gitRevisionFilePath);
    private:
        FilePathManager();
        void InitAppName();
        void InitPublishConfigFilePath();
        void InitLogsFolderPath();
        void InitTemplateFilePath();
        void InitGitRevisionFilePath();
        void InitResourceFilePath();

    private:
        std::wstring m_appName;
        FilePath m_publishConfigFilePath;
        FilePath m_behaviorConfigFilePath;
        FilePath m_logsFolderPath;
        FilePath m_templateFolderPath;
        FilePath m_gitRevisionFilePath;
        FilePath m_resourceFilePath;

        bool m_isInitialled;
    };
}

