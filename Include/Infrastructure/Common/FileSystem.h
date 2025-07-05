#pragma once

#include <io.h>
#include <Windows.h>
#include "OwnerPtr.h"
#include "JDefines.h"
#include "JCommonDefine.h"
#include "FileUtils.h"
#include "StringUtils.h"


namespace JackC
{
    struct FileDeleter
    {
        void operator()(FILE* p) const
        {
            //FILE* 文件指针
            fclose(p);    //fclose关闭文件
        };
    };

    typedef OwnerPtr<FILE, FileDeleter> FileOwnerPtr;



    namespace FileSystem
    {
        //文件路径分隔符
        FORCE_INLINE const std::wstring Separator()
        {
            const std::wstring s_sep = L"\\";
            return s_sep;
        }
        FORCE_INLINE static const wchar_t SeparatorChar()
        {
            return L'\\';
        }


        JACKC_COMMON_INTERFACE_EXPORT FileOwnerPtr OpenFile(const std::wstring& path, const std::wstring& mode);

        //读取文件到流中 ifstream 读取文件到流
        JACKC_COMMON_INTERFACE_EXPORT bool OpenStream(std::ifstream& stream, const std::wstring& path, std::ios_base::openmode mode = std::ios_base::in);
        //对文件进行读取或者写入
        JACKC_COMMON_INTERFACE_EXPORT bool OpenStream(std::fstream& stream, const std::wstring& path, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);
        //数据写入文件
        JACKC_COMMON_INTERFACE_EXPORT bool OpenStream(std::ofstream& stream, const std::wstring& path, std::ios_base::openmode mode = std::ios_base::out);
        //数据写入文件
        JACKC_COMMON_INTERFACE_EXPORT bool OpenStream(std::wofstream& stream, const std::wstring& path, std::ios_base::openmode mode = std::ios_base::out);
        //创建目录，不能一次递归创建多级
        JACKC_COMMON_INTERFACE_EXPORT bool CreateDir(const std::wstring& path);


        FORCE_INLINE std::wstring GetExeDirPath()
        {
            return FileUtils::GetExeDirPath();
        }
        FORCE_INLINE std::wstring GetCurrentProcessID()
        {
            DWORD pid = GetCurrentProcessId();
            return StringUtils::ToWString(pid);
        }
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetSdkDirPath();
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetTempDirPath();
        // <user name>\Local Settings\Applicaiton Data (non roaming)\AppCube\
            // 以文件分隔符结尾
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetLocalAppDataDirPath(const std::wstring& appName = L"AppCube");

        // C:\Users\Administrator\Documents
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetLocalDocumentDirPath(const std::wstring& appName = L"Documents");

        // <user name>\Applicaiton Data (non roaming)
        // 以文件分隔符结尾
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetAppDataDirPath();

        // 获取工作目录
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetWorkingDirPath();

        // 获取测试目录
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetTestDirPath();

        /// \brief 获取系统磁盘下的ProgramData文件目录
        ///
        /// \return 路径的字符串
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetCommonAppDataDirPath();


        // 统一路径分隔符，去除多余路径分隔符
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetPurePath(const std::wstring& path);
        JACKC_COMMON_INTERFACE_EXPORT std::wstring GetWorkingDirPath();

        typedef void* ModuleHandle;
        //加载模块
        JACKC_COMMON_INTERFACE_EXPORT ModuleHandle LoadModule(const std::wstring& modulePath);

        JACKC_COMMON_INTERFACE_EXPORT bool RemoveFile(const std::wstring& path);
        JACKC_COMMON_INTERFACE_EXPORT bool RemoveDir(const std::wstring& path);

    }

    //文件信息类，用于：判断是文件还是文件夹，文件大小，文件是否存在
    class JACKC_COMMON_INTERFACE_EXPORT FileInfo
    {
    private:
        bool m_exists;
        bool m_isFile;
        bool m_isDirectory;
        int64_t m_size;
    public:
        FileInfo(const std::wstring& driveOrPath);
        ~FileInfo()
        {
            int t = 1;
        }
    public:
        bool Exists() { return m_exists; }
        bool IsFile() { return m_isFile; }
        bool IsDirectory() { return m_isDirectory; }

        int64_t Size() { return m_size; }
    };


#pragma warning(push)
#pragma warning(disable:4251)
    class JACKC_COMMON_INTERFACE_EXPORT  DirectoryEntry
    {
    public:
        enum EmDirectoryEntryType
        {
            DET_NotSupport,
            DET_NormalFile,
            DET_Directory,
        };
    private:
        std::wstring m_parentPath;
        std::wstring m_name;
        EmDirectoryEntryType m_type;
    public:
        //返回文件或目录名
        const std::wstring& GetName() const { return m_name; }
        //返回全路径
        std::wstring GetFullName() const { return FileSystem::GetPurePath(m_parentPath + FileSystem::Separator() + m_name); }

        EmDirectoryEntryType GetType() const { return m_type; }
        bool IsFile() const { return (m_type == DET_NormalFile); }
        bool IsDirectory() const { return (m_type == DET_Directory); }

    public:
        friend class  DirectoryIterator;

    };
#pragma warning(pop)

    class JACKC_COMMON_INTERFACE_EXPORT DirectoryIterator
    {
    public:
        DirectoryIterator();
        explicit DirectoryIterator(const std::wstring& path);
        ~DirectoryIterator();

    private:
        bool m_isOpen;
        DirectoryEntry m_currentEntry;
        intptr_t m_hDir;
        struct _wfinddata_t m_dirData;  //-wfinddata_t:结构用来记录查找到的文件的信息
    public:
        const DirectoryEntry& operator*() const;
        const DirectoryEntry* operator->() const;

        DirectoryIterator& operator++();

        bool operator!=(const DirectoryIterator& oteher);
        bool operator==(const DirectoryIterator& oteher);
    };
}
