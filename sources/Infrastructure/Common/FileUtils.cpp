#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "FileUtils.h"
#include "FileSystem.h"
#include "simdutf8check.h"
#include "FilePath.h"

//
using namespace JackC;

std::vector<std::wstring> FileUtils::GetFiles(const std::wstring& path, const std::wstring& ext)
{
    std::vector<std::wstring> files;
    FileInfo fileInfo(path);
    if (!fileInfo.IsDirectory())
        return files;
    for (DirectoryIterator iter(path), end; iter != end; ++iter)
    {
        std::wstring name = iter -> GetName();
        if (name == L"." || name == L".." || !iter->IsFile()) continue;
        FilePath filePath(name);
        if (filePath.GetExtension() == ext)
        {
            files.push_back(name);
        }
    }
    return files;
}
std::vector<std::wstring> FileUtils::GetFilePathsByFilter(const std::wstring& path, std::function<bool(const DirectoryEntry& entry)> filter, bool recursively)
{
    std::vector<std::wstring> res;
    if (FileInfo(path).IsFile())
    {
        return res;
    }
    for (DirectoryIterator iter(path), end; iter != end; ++iter)
    {
        if (L"." == iter->GetName() || L".." == iter->GetName())
        {
            continue;
        }
        if (!filter || filter(*iter))
        {
            res.push_back(iter->GetFullName());
        }
        if (recursively &&
            iter->IsDirectory())
        {
            std::vector<std::wstring> subDirRes = GetFilePathsByFilter(iter->GetFullName(), filter, recursively);
            if (!subDirRes.empty())
            {
                res.insert(res.end(), subDirRes.begin(), subDirRes.end());
            }
        }
    }
    return res;
}

bool FileUtils::AppendHideAttribute(const std::wstring& filePath)
{
#ifdef _WINDOWS
    DWORD dwFileAttributes = GetFileAttributes(filePath.c_str());
    dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
    return SetFileAttributes(filePath.c_str(), dwFileAttributes) ? true : false;
#else
    // Not implemented
    return true;
#endif
}
bool FileUtils::RemoveHideAttribute(const std::wstring& filePath)
{
#ifdef _WINDOWS
    DWORD dwFileAttributes = GetFileAttributes(filePath.c_str());
    dwFileAttributes &= ~FILE_ATTRIBUTE_HIDDEN;
    return SetFileAttributes(filePath.c_str(), dwFileAttributes) ? true : false;
#else
    // Not implemented
    return true;
#endif
}

//获取路径分隔符
const char FileUtils::GetPathSeparatorChar()
{
    return '\\';
}
//获取当前exe路径
std::wstring FileUtils::GetExeDirPath()
{
    wchar_t pPath[MAX_PATH];
    //获取exe可执行文件的绝对路径
    GetModuleFileName(NULL, pPath, MAX_PATH);
    std::wstring strExePath = pPath;
    return strExePath.substr(0, strExePath.find_last_of(GetPathSeparatorChar()) + 1);
}
std::wstring FileUtils::GetAvailableFilePath(const std::wstring& fileFolder, const std::wstring& prefix, const std::wstring& ext)
{
    for (int index = 1; index < 9999; ++index)
    {
        std::wostringstream stream;
        stream << fileFolder << prefix << L"_" << index << ext;
        std::wstring filePath = stream.str();
        if (!FileInfo(filePath).Exists())
        {
            return filePath;
        }
    }
    return L"";
}


//根据路径获取文件信息
std::vector<char> FileUtils::Load(const std::wstring& filePath)
{
    std::ifstream stream;
    std::vector<char> result;
    FileSystem::OpenStream(stream, filePath);   //读取文件到流
    bool isOpen=stream.is_open();   //检查文件是否打开
    if (!isOpen)
        std::move(result);
    int dataSize = (int)FileInfo(filePath).Size();
    result.resize(dataSize);
    stream.read(result.data(), dataSize);
    stream.close();
    return std::move(result);
}
//根据路径获取文件信息
std::vector<char> FileUtils::LoadInBinaryMode(const std::wstring& filePath)
{
    std::ifstream stream;
    std::vector<char> result;
    FileSystem::OpenStream(stream, filePath,std::ios_base::in|std::ios_base::binary);   //读取文件到流
    bool isOpen = stream.is_open();   //检查文件是否打开
    if (!isOpen)
        std::move(result);
    int dataSize = (int)FileInfo(filePath).Size();
    result.resize(dataSize);
    stream.read(result.data(), dataSize);
    stream.close();
    return std::move(result);
}
std::string FileUtils::LoadINIFile(const std::wstring& filePath)
{
    FileInfo fileInfo(filePath);
    if (!fileInfo.IsFile()) return "";

    std::ifstream stream;
    std::vector<char> retData;
    FileSystem::OpenStream(stream, filePath, std::ios_base::in | std::ios_base::binary);
    bool isOpen = stream.is_open();
    if (!isOpen) std::move(retData);

    int dataSize = (int)fileInfo.Size();
    retData.resize(dataSize);
    stream.read(retData.data(), dataSize);
    stream.close();

    if (validate_utf8_fast(retData.data(), dataSize))
    {
        // Remove BOM (Byte Order Mark)
        const char* utf8bom = "\xef\xbb\xbf";
        char* strBuffer = retData.data();
        if (!std::string(strBuffer, 3).compare(utf8bom))
        {
            strBuffer += 3;
        }
        return strBuffer;
    }
    else
    {
        //DBG_WARN(L"INI文件内容格式无效");
        return "";
    }
}


bool FileUtils::Save(const std::wstring& filePath, const char* data, int dataSize)
{
    std::ofstream stream;
    FileSystem::OpenStream(stream, filePath);
    //GetLastError()返回调用线程最近的错误代码值，错误代码以单线程为基础来维护的。
    int errorMode = GetLastError();
    bool isOpen = stream.is_open();
    std::wostringstream woss;
    woss << filePath << L"文件打开失败，错误码是" << GetLastError();
    //FYI(woss.str());
    stream.write(data, dataSize);
    stream.close();
    return true;

}
bool FileUtils::SaveInBinaryMode(const std::wstring& filePath, const char* data, int dataSize, bool isAppend)
{
    std::ofstream stream;
    int mode = std::ios_base::out | std::ios_base::binary;
    if (isAppend)
    {
        mode |= std::ios_base::app;
    }
    FileSystem::OpenStream(stream, filePath, mode);
    bool isOpen = stream.is_open();
    stream.write(data, dataSize);
    stream.close();
    return true;
}

bool FileUtils::CreateDirectoriesImpl(const std::wstring& path)
{
    if (FileInfo(path).IsDirectory()) return true;

    auto pos = path.find_last_of(FileSystem::SeparatorChar());
    if (pos == std::wstring::npos)
    {
        // 没有父目录了
        return FileSystem::CreateDir(path);
    }
    else
    {
        // 创建父目录
        const std::wstring parentPath = path.substr(0, pos);
        if (CreateDirectoriesImpl(parentPath))
        {
            // 创建目录
            return FileSystem::CreateDir(path);
        }
        return false;
    }
}
bool FileUtils::CreateDirectories(const std::wstring& path)
{
    const std::wstring purePath = FileSystem::GetPurePath(path);
    return CreateDirectoriesImpl(purePath);
}

int FileUtils::RemoveAll(const std::wstring& path)
{
    std::wstring purePath = FileSystem::GetPurePath(path);
    if (FileInfo(purePath).Exists())
    {
        auto removeAllFilter = [](const DirectoryEntry& entry) {return true; };
        return RemoveAllImpl(purePath, removeAllFilter);
    }
    return 0;
}
int FileUtils::RemoveAll(const std::wstring& path, const std::function<bool(const DirectoryEntry& entry)>& filter)
{
    std::wstring purePath = FileSystem::GetPurePath(path);
    if (FileInfo(purePath).Exists())
    {
        return RemoveAllImpl(purePath, filter);
    }
    return 0;
}
int FileUtils::RemoveAllImpl(const std::wstring& path, const std::function<bool(const DirectoryEntry& entry)>& filter)
{
    int ret = 0;
    FileInfo fileInfo(path);
    if (fileInfo.IsFile())
    {
        FileSystem::RemoveFile(path);
        ++ret;
    }
    else if (fileInfo.IsDirectory())
    {
        for (DirectoryIterator iter(path), end; iter != end; ++iter)
        {
            std::wstring name = iter->GetName();
            if (name == L"." || name == L".." || !filter(*iter))
                continue;
            // 递归删除
            ret += RemoveAllImpl(iter->GetFullName(), filter);
        }
        if (FileSystem::RemoveDir(path))
        {
            ++ret;
        }
    }

    return ret;
}

void FileUtils::CopyImpl(const std::wstring& srcPath, const std::wstring & destPath,const std::function<bool(const DirectoryEntry& entry)>& filter)
{
    FileInfo fileInfo(srcPath);
    if (fileInfo.IsFile())
    {
        CopyFileW(srcPath, destPath);
    }
    else if (fileInfo.IsDirectory())
    {
        FileSystem::CreateDir(destPath);
        for (DirectoryIterator iter(srcPath), end; iter != end; ++iter)
        {
            std::wstring name = iter->GetName();
            if (name == L"." || name == L".." || !filter(*iter)) continue;
            CopyImpl(iter->GetFullName(), destPath + FileSystem::Separator() + iter->GetName(), filter);
        }
    }
}
void FileUtils::Copy(const std::wstring& srcPath, const std::wstring& destPath)
{
    std::wstring pureSrcPath = FileSystem::GetPurePath(srcPath);
    std::wstring pureDestPath = FileSystem::GetPurePath(destPath);
    if (FileInfo(pureSrcPath).Exists())
    {
        auto copyAllFilter = [](const DirectoryEntry& entry) {return true; };
        CopyImpl(pureSrcPath, pureDestPath, copyAllFilter);
    }
}
void FileUtils::Copy(const std::wstring& srcPath, const std::wstring& destPath, const std::function<bool(const DirectoryEntry& entry)>& filter)
{
    std::wstring pureSrcPath = FileSystem::GetPurePath(srcPath);
    std::wstring pureDestPath = FileSystem::GetPurePath(destPath);
    if (FileInfo(pureSrcPath).Exists())
    {
        CopyImpl(pureSrcPath, pureDestPath, filter);
    }
}
bool FileUtils::CopyFileW(const std::wstring& srcFile, const std::wstring& destFile, bool bFailIfExists)
{
#ifdef _WINDOWS
    return ::CopyFileW(srcFile.c_str(), destFile.c_str(), bFailIfExists) != FALSE;
#else

#endif
    return true;
}