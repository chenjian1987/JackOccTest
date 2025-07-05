#include "FilePath.h"
#include "FileSystem.h"
#include "DebugMessage.h"
#include "FilePathManager.h"
#include "IBehaviorConfig.h"
#include <iostream>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;


FilePathManager* FilePathManager::Get()
{
    static FilePathManager s_instance;
    return &s_instance;
}

FilePathManager::FilePathManager()
    : m_publishConfigFilePath(L"")
    , m_behaviorConfigFilePath(L"")
    , m_logsFolderPath(L"")
    , m_templateFolderPath(L"")
    , m_gitRevisionFilePath(L"")
    , m_isInitialled(false)
    , m_resourceFilePath(L"")
{

}

FilePathManager::~FilePathManager(void)
{
}

void FilePathManager::Init()
{
    InitPublishConfigFilePath();
    InitLogsFolderPath();
    InitTemplateFilePath();
    InitGitRevisionFilePath();
    InitAppName();
    InitResourceFilePath();
    m_isInitialled = true;
}

const FilePath& FilePathManager::GetPublicConfigFilePath()
{
    if (!m_isInitialled)
    {
        InitPublishConfigFilePath();
    }
    return m_publishConfigFilePath;
}

const FilePath& FilePathManager::GetBehaviorConfigFilePath()
{
    if (!m_isInitialled)
    {
        InitPublishConfigFilePath();
    }
    return m_behaviorConfigFilePath;
}

void FilePathManager::SetBehaviorConfigFilePath(const std::wstring& behaviorConfigFilePath)
{
    m_behaviorConfigFilePath = behaviorConfigFilePath;
    IBehaviorConfig::SetBehaviorConfigPath(behaviorConfigFilePath);
}

const FilePath& FilePathManager::GetLogsFolderPath()
{
    if (!m_isInitialled)
    {
        Init();
    }
    return m_logsFolderPath;
}

void FilePathManager::SetLogsFolderPath(const std::wstring& logsFolderPath)
{
    m_logsFolderPath = logsFolderPath;
}

const FilePath& FilePathManager::GetTemplateFolderPath()
{
    if (!m_isInitialled)
    {
        Init();
    }
    return m_templateFolderPath;
}

void FilePathManager::SetTemplateFolderPath(const std::wstring& templateFolderPath)
{
    m_templateFolderPath = templateFolderPath;
}

const FilePath& FilePathManager::GetGitRevisionFilePath()
{
    if (!m_isInitialled)
    {
        InitPublishConfigFilePath();
    }
    return m_gitRevisionFilePath;
}

void FilePathManager::SetGitRevisionFilePath(const std::wstring& gitRevisionFilePath)
{
    m_gitRevisionFilePath = gitRevisionFilePath;
}

void FilePathManager::InitAppName()
{
    m_appName = IBehaviorConfig::GetConfigRecordValueAsString(BehaviorConfigRecordNames::ClientAppName);
}

void FilePathManager::InitPublishConfigFilePath()
{
    m_publishConfigFilePath = FileSystem::GetSdkDirPath() + L"jack_public_config.xml";
}

void FilePathManager::InitLogsFolderPath()
{
    // 内部版本输出到Exe同级目录下
    m_logsFolderPath = FileSystem::GetExeDirPath() + L"Logs";
}

void FilePathManager::InitTemplateFilePath()
{
    std::wstring templateFolderPath = FileSystem::GetExeDirPath();
    m_templateFolderPath = templateFolderPath + L"Templates" + FileSystem::Separator();
}


void FilePathManager::InitGitRevisionFilePath()
{
    m_gitRevisionFilePath = FileSystem::GetExeDirPath() + L"Revision.txt";
}

void FilePathManager::InitResourceFilePath()
{
    static std::wstring wsExePath = FileSystem::GetExeDirPath();
    m_resourceFilePath = wsExePath + L"Resource";
}

const FilePath& FilePathManager::GetResourcePath()
{
    if (!m_isInitialled)
    {
        InitResourceFilePath();
    }
    return m_resourceFilePath;
}
