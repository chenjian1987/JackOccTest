#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include "JDevService.h"
#include "FileSystem.h"
#include "FilePath.h"
#include "DebugMessage.h"
#include "LogManager.h"
#include "EnvironmentVariables.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include <ShlObj.h>

using namespace JackC;

namespace
{
#ifdef _WINDOWS
    void ShowDebugWarnOnError(DWORD dwError, const std::wstring& errorMessage)
    {
        HLOCAL hlocal = NULL;
        //根据主要语言标识符和子语言标识符创建语言标识符
        DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
        //format将GetLastError函数得到的错误信息转化为字符串信息的函数
        BOOL fOk = FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
            FORMAT_MESSAGE_ALLOCATE_BUFFER,
            NULL, dwError, systemLocale,
            (PTSTR)&hlocal, 0, NULL
        );
        if (fOk && (hlocal != NULL))
        {
            std::wostringstream woss;
            woss << errorMessage << L"时发生错误：" << (PCTSTR)hlocal;
            //DBG_WARN(woss.str());
            LocalFree(hlocal);
            return;
        }
    }
#endif
}

std::wstring FileSystem::GetSdkDirPath()
{
    //GetModuleHandle获取exe或dll 的句柄
    HMODULE hModuleHandle = GetModuleHandle(L"Common.dll");
    wchar_t pPath[MAX_PATH];
    if (hModuleHandle != NULL)
        GetModuleFileName(hModuleHandle, pPath, MAX_PATH);
    else
    {
    }
    FilePath fp(pPath);
    return fp.GetDirectoryName();
}

std::wstring FileSystem::GetTempDirPath()
{
#ifdef _WINDOWS
    wchar_t tempPath[MAX_PATH];
    GetTempPath(MAX_PATH, tempPath);
    return tempPath;
#else
    return L"/tmp/";
#endif
}

std::wstring FileSystem::GetLocalDocumentDirPath(const std::wstring& appName)
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    if (SHGetSpecialFolderPath(0, appDataPath, CSIDL_PROFILE, FALSE))
    {
        std::wstring appPath = L"\\";
        if (!appName.empty())
        {
            appPath = L"\\" + appName + L"\\";
        }
        wcscat_s(appDataPath, appPath.c_str());
        if (!FileInfo(appDataPath).IsDirectory())
        {
            FileSystem::CreateDir(appDataPath);
        }

        return appDataPath;
    }
    else
    {
        return GetTempDirPath();
    }
#else
return L"./home/";
#endif
}

std::wstring FileSystem::GetLocalAppDataDirPath(const std::wstring& appName)
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    if (SHGetSpecialFolderPath(0, appDataPath, CSIDL_LOCAL_APPDATA, FALSE))
    {
        std::wstring appPath = L"\\";
        if (!appName.empty())
        {
            appPath = L"\\" + appName + L"\\";
        }
        wcscat_s(appDataPath, appPath.c_str());
        if (!FileInfo(appDataPath).IsDirectory())
        {
            FileSystem::CreateDir(appDataPath);
        }

        return appDataPath;
    }
    else
    {
        return GetTempDirPath();
    }
#else
    return L"./home/";
#endif
}

std::wstring FileSystem::GetAppDataDirPath()
{
#ifdef _WINDOWS
    wchar_t appDataPath[MAX_PATH];
    memset(appDataPath, 0, sizeof(wchar_t) * MAX_PATH);

    ::SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA, FALSE);
    if (!FileInfo(appDataPath).IsDirectory())
    {
        FileSystem::CreateDir(appDataPath);
    }
    return appDataPath;
#else
    return L"./home/";
#endif
}

std::wstring FileSystem::GetWorkingDirPath()
{
#ifdef _WINDOWS
    wchar_t tempPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, tempPath);
    return tempPath;
#else
    // TODO: Implement
    return L"./";
#endif
}

std::wstring FileSystem::GetTestDirPath()
{
    return FilePath::Combine(GetExeDirPath(), L"../../tests");
}

std::wstring FileSystem::GetCommonAppDataDirPath()
{
#ifdef _WINDOWS_
    wchar_t commonAppDataPath[MAX_PATH];
    memset(commonAppDataPath, 0, MAX_PATH);

    ::SHGetSpecialFolderPath(NULL, commonAppDataPath, CSIDL_COMMON_APPDATA, false);
    if (!FileInfo(commonAppDataPath).IsDirectory())
    {
        FileSystem::CreateDir(commonAppDataPath);
    }
    return commonAppDataPath;
#else
    return L"./home/";
#endif
}

FileSystem::ModuleHandle FileSystem::LoadModule(const std::wstring& modulePath)
{
    ModuleHandle res = nullptr;

#ifdef _WINDOWS
    res = ::LoadLibraryW(modulePath.c_str());   //指定的模块加载到调用进程的地址空间中

    DWORD errorCode = GetLastError();
    if (res == nullptr && errorCode != 0)
    {
        FYI(L"Load module-window last error:" + StringUtils::ToWString(errorCode) + L",dll path=" + modulePath);
    }
    if (res == nullptr)
    {
        std::wstring absPath(modulePath);
        if (FilePath::IsRelativePath(absPath))
        {
            absPath = FilePath::Combine(GetExeDirPath(), modulePath);
        }
        FilePath fp(absPath);
        std::wstring dirPath = EnvironmentVariables::AddPath(fp.GetDirectoryName());
        EnvironmentVariableSetter envSetter(L"Path", dirPath);
        res = ::LoadLibraryW(absPath.c_str());

        DWORD errorCode = GetLastError();
        if (res == nullptr && errorCode != 0)
        {
            FYI(L"Loading module-WIndow last error is: " + StringUtils::ToWString(errorCode) + L",dirpath is:" + dirPath);
        }
    }
#endif
    return res;
}

FileOwnerPtr FileSystem::OpenFile(const std::wstring  &path,const std::wstring &mode)
{
    FILE* fp = nullptr;
    _wfopen_s(&fp, path.c_str(), mode.c_str());
    return FileOwnerPtr(fp);
}

//ofstream 将流中的数据写入文件
//ifstream 将文件的数据读入到流中
//fstream 对文件进行读取和写操作
bool FileSystem::OpenStream(std::ifstream& stream, const std::wstring& path, std::ios_base::openmode mode)
{
    stream.open(path, mode);
    return stream.good();
}
bool FileSystem::OpenStream(std::ofstream& stream, const std::wstring& path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}
bool FileSystem::OpenStream(std::wofstream& stream, const std::wstring& path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}
bool FileSystem::OpenStream(std::fstream& stream, const std::wstring& path, std::ios_base::openmode mode)
{
#ifdef _WINDOWS
    stream.open(path, mode);
#else
    stream.open(StringUtils::ToString(path), mode);
#endif
    return stream.good();
}

bool FileSystem::CreateDir(const std::wstring &path)
{
    if (FileInfo(path).Exists()) return false;
//#ifdef _WINDOWS
    if (_wmkdir(path.c_str()) != 0)  //创建一个目录，成功返回0，失败返回-1
    {
        ShowDebugWarnOnError(GetLastError(), L"创建目录" + path);
        return false;
    }
    return true;
//#endif
}
std::wstring FileSystem::GetPurePath(const std::wstring& path)
{
    std::wstring retPath;
#ifdef _WINDOWS
    const wchar_t from = L'/';
    const wchar_t to = L'\\';
#else
    const wchar_t from = L'\\';
    const wchar_t to = L'/';
#endif
    wchar_t last = L'\0';
    for (int i = 0; i < (int)path.size(); ++i)
    {
        wchar_t ch = path[i];
        if (ch == from)
            ch = to;
        // 去掉连续的分隔符
        if (last == SeparatorChar() && ch == SeparatorChar())
            continue;
        retPath.push_back(ch);
        last = ch;
    }
    // 去掉末尾的分隔符
    if (last == SeparatorChar())
        retPath.pop_back();
    return retPath;
}

bool FileSystem::RemoveFile(const std::wstring& path)
{
    //FYI(L"RemoveFile:" + path);
    // 文件不存在，直接返回true
    if (!FileInfo(path).Exists()) return true;

#ifdef _WINDOWS
    if (0 == _wremove(path.c_str()))
    {
        return true;
    }
    int lastError = GetLastError();
    return false;
#else
    return remove(StringUtils::ToString(path).c_str());
#endif
}
bool FileSystem::RemoveDir(const std::wstring& path)
{
#ifdef _WINDOWS
    //FYI(L"removedir:" + path);
    return (_wrmdir(path.c_str()) == 0);
#else
    std::string sPath = StringUtils::ToString(path);
    return (rmdir(sPath.c_str()) == 0);
#endif
}

FileInfo::FileInfo(const std::wstring& driveOrPath):m_exists(false),m_isFile(false),m_isDirectory(false),m_size(0)
{
    /*
       int _waccess(const wchar_t* path,int mode) 
         path:文件或目录路径
         mode访问权限设定
         返回值：如果文件具有指定的访问权限，则函数返回0，如果文件不存在或者不能访问指定的权限，则返回-1.
         当path为文件时，__access函数判断文件是否存在，并判断文件是否可以用mode值指定的模式进行访问。 当path为目录时，_access只判断指定的目录是否存在.
         mode: 0检查文件是否存在   2写权限   4读取权限  6读取和写入权限
      */
    m_exists = (_waccess(driveOrPath.c_str(), 0)==0);
    if (m_exists)
    {
        const std::wstring name = (driveOrPath.length() == 2 && driveOrPath.at(1) == L':') ? driveOrPath + FileSystem::Separator() : driveOrPath;
        struct __stat64 temp = { 0 };
        /*
         _wstat64获取有关描述符的文件状态信息
          参数：path 至现有文件或目录的路径字符串的指针  buffer结构化指针存储 
          返回值：文件则表示信息获取到，返回0 ，返回-1 表示错误

           path 或 _stat 函数获取有关目录的信息指定的文件并将其存储结构中指向的 buffer。
          _stat它们自动处理合适的多字节字符串参数，根据当前使用的多字节代码页识别多字节字符序列.
        */
        if (_wstat64(name.c_str(), &temp) == 0)
        {
            //st_mode   文件模式信息的位掩码。如果路径指定了一个目录，则设置_S_IFDIR位；如果路径指定普通文件或设备，
            //            则设置_。根据文件的权限模式设置用户读/写位；根据文件扩展名设置用户执行位
            //S_IFREG代表普通文件
            m_isFile = ((S_IFREG & temp.st_mode) != 0);
            m_isDirectory = ((S_IFDIR & temp.st_mode) != 0);
            m_size = temp.st_size;
        }
    }
}


DirectoryIterator::DirectoryIterator():m_isOpen(false)
{
}
DirectoryIterator::DirectoryIterator(const std::wstring& path):m_isOpen(false)
{
    m_currentEntry.m_parentPath = path;
#ifdef _WINDOWS
    std::wstring dirPath = path;
    //_wfindfirst搜索指定路径下的第一个文件
    m_hDir = _wfindfirst(dirPath.append(L"\\*").c_str(), &m_dirData);
    if (m_hDir != -1L)
    {
        m_isOpen = true;
        m_currentEntry.m_name = m_dirData.name;
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        if ((m_dirData.attrib & _A_HIDDEN) == 0)
        {
            if (m_dirData.attrib & _A_SUBDIR) type = DirectoryEntry::DET_Directory;
            else type = DirectoryEntry::DET_NormalFile;
        }
        m_currentEntry.m_type = type;
    }
#endif


}
DirectoryIterator::~DirectoryIterator()
{
    if (m_isOpen)
    {
#ifdef _WINDOWS
        _findclose(m_hDir);
#else 
#endif
        m_isOpen = false;
    }
}
const DirectoryEntry& DirectoryIterator::operator*() const
{
    return m_currentEntry;
}
const DirectoryEntry* DirectoryIterator::operator->() const
{
    return &m_currentEntry;
}
DirectoryIterator& DirectoryIterator::operator++()
{
    if (!m_isOpen)
        return *this;
#ifdef _WINDOWS
    if (_wfindnext(m_hDir, &m_dirData) == 0)
    {
        m_currentEntry.m_name = m_dirData.name;
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        // TODO 暂时忽略隐藏文件
        if ((m_dirData.attrib & _A_HIDDEN) == 0)
        {
            if (m_dirData.attrib & _A_SUBDIR) type = DirectoryEntry::DET_Directory;
            else type = DirectoryEntry::DET_NormalFile;
        }
        m_currentEntry.m_type = type;
    }
    else
    {
        m_isOpen = false;
        _findclose(m_hDir);
    }
#else
    struct dirent* ptr = readdir(m_dirData);
    if (ptr)
    {
        m_currentEntry.m_name = StringUtils::ToWString(ptr->d_name);
        DirectoryEntry::EmDirectoryEntryType type = DirectoryEntry::DET_NotSupport;
        if (ptr->d_type == DT_DIR) type = DirectoryEntry::DET_Directory;
        else if (ptr->d_type == DT_REG) type = DirectoryEntry::DET_NormalFile;

        m_currentEntry.m_type = type;
    }
    else
    {
        m_isOpen = false;
        closedir(m_dirData);
    }
#endif
    return *this;
}
bool DirectoryIterator::operator!=(const DirectoryIterator& other)
{
    return !(*this == other);
}
bool DirectoryIterator::operator==(const DirectoryIterator& other)
{
    // 和自身相等；目录都未打开时相等；其它情况暂视为不相等
    if (!m_isOpen && !other.m_isOpen)
        return true;
    if (this == &other)
        return true;
    return false;
}