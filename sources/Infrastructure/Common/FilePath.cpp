#include <cstdarg>
#include <iostream>

#include "FilePath.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include <Shlwapi.h>
#include "FileSystem.h"

using namespace JackC;

FilePath::FilePath(const std::wstring& filePath)
{
    std::wstring tempFilePath = filePath;
    size_t nPos = filePath.find(L"\\\\?\\");
    if (nPos == 0)
        tempFilePath = tempFilePath.substr(4);
    wchar_t drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    //_wsplitpath:可以从路径中 分解出盘符、目录、文件名、文件后缀等
    _wsplitpath(tempFilePath.c_str(), drive, dir, fname, ext);

    m_drive = drive;
    m_directory = dir;
    m_filename = fname;
    m_extension = ext;
}

FilePath::~FilePath()
{
    int t = 0;
}

FilePath::FilePath(FilePath& file)
{
    auto t = file;
}

std::wstring FilePath::GetDirectoryName() const
{
    //_MAX_PATH:编译器所支持的最长路径名的长度，一般是260
    wchar_t pathBuffer[_MAX_PATH];
    //_wmakepath_s从组建创建路径名，参数1：完整路径缓冲区，  参数2：缓冲区的字大小。  参数3：包含与预期驱动器和一个选项尾部冒号对应的字母（A,B等），
    //   参数4：目录路径  
    _wmakepath_s(pathBuffer, _MAX_PATH, m_drive.c_str(), m_directory.c_str(), NULL, NULL);
    return pathBuffer;
}

std::wstring FilePath::GetExtension() const
{
    return m_extension;
}
std::wstring FilePath::GetFileNameWithoutExtension() const
{
    return m_filename;
}
std::wstring FilePath::GetFileName() const
{
    wchar_t fileNameBuffer[_MAX_PATH];
    _wmakepath_s(fileNameBuffer, _MAX_PATH, NULL, NULL, m_filename.c_str(), m_extension.c_str());
    return fileNameBuffer;
}
std::wstring FilePath::GetFullPath() const
{
    wchar_t pathBuffer[_MAX_PATH];
    _wmakepath_s(pathBuffer, _MAX_PATH, m_drive.c_str(), m_directory.c_str(), m_filename.c_str(), m_extension.c_str());
    return pathBuffer;
}
std::wstring FilePath::GetPathRoot() const
{
    return m_drive;
}
bool FilePath::HasExtension() const
{
    return !m_directory.empty();
}

namespace
{
    std::wstring ConcatPathPart(const std::wstring& part) { return part; }

    template <class T>
    T ConcatPathParts(const T& t)
    {
        return ConcatPathPart(t);
    }

    template <class T,class ...Ts>
    T ConcatPathParts(const T& t, const Ts &...args)
    {
        return ConcatPathPart(t) + FileSystem::Separator() + ConcatPathParts(args...);
    }
}

std::wstring FilePath::Combine(const std::wstring& part1, const std::wstring& part2)
{
    std::wstring combinePath = ConcatPathParts(part1, part2);
    return FileSystem::GetPurePath(combinePath);
}
std::wstring FilePath::Combine(const std::wstring& part1, const std::wstring& part2, const std::wstring& part3)
{
    std::wstring combinedPath = ConcatPathParts(part1, part2, part3);
    return FileSystem::GetPurePath(combinedPath);
}

bool FilePath::IsRelativePath(const std::wstring& path)
{
    if (path.empty())
        return true;
    if (path[0] == L'/')
        return false;
    if (path[0] == L':')
        return false;
    if (path.size() > 1 && path[1] == L':')
        return false;
    return true;
}

bool FilePath::IsOnSameDrive(const std::wstring& path1, const std::wstring& path2)
{
    if (path1.size() >= 2 && path2.size() >= 2 && path1[1] == L':' && path2[1] == L':' && path1[0] != path2[0])
        return false;
    return true;
}

std::wstring FilePath::GetAbsolutePath(const std::wstring& relativePath, const std::wstring& parentDir)
{
    if (!IsRelativePath(relativePath))
        return relativePath;
    std::wstring parentDirTemp;
    if (parentDir.empty())
        parentDirTemp = FileSystem::GetWorkingDirPath();
    else
        parentDirTemp = parentDir;
    return FilePath::Combine(parentDirTemp, relativePath);
}

std::wstring FilePath::GetRelativePath(const std::wstring& fullPath, const std::wstring& parentDir)
{
#ifdef _WINDOWS
    wchar_t szOut[_MAX_PATH] = L"";
    std::wstring fullPathPure = FileSystem::GetPurePath(fullPath);
    std::wstring parentDirPure = FileSystem::GetPurePath(parentDir);
    PathRelativePathTo(szOut, parentDirPure.data(), FILE_ATTRIBUTE_DIRECTORY, fullPathPure.data(), FILE_ATTRIBUTE_NORMAL);
    return szOut;
#else
    return fullPath;
#endif
}