#pragma once
#include "JDevService.h"
#include "JCommonDefine.h"

namespace JackC
{
	class DirectoryEntry;

	class JACKC_COMMON_INTERFACE_EXPORT FileUtils
	{
	private:
		FileUtils(void);
	private:
		//路径存在或创建成功，返回true
		static bool CreateDirectoriesImpl(const std::wstring& path);
		static int RemoveAllImpl(const std::wstring& path, const std::function<bool(const DirectoryEntry& entry)>& filter);
		static void CopyImpl(const std::wstring& srcPath, const std::wstring& destPath, const std::function<bool(const DirectoryEntry& entry)>& filter);

	public:
		static const char GetPathSeparatorChar();
		static std::wstring GetExeDirPath();
		static bool CopyFileW(const std::wstring& srcFile, const std::wstring& prefix, bool bFailIfExists = false);
		static std::wstring GetAvailableFilePath(const std::wstring& fileFolder, const std::wstring& prefix, const std::wstring& ext);

		static std::vector<char> Load(const std::wstring& filePath);
		static std::vector<char> LoadInBinaryMode(const std::wstring& filePath);
		static std::string LoadINIFile(const std::wstring& filePath);

		static bool Save(const std::wstring& filePath, const char* data, int dataSize);
		static bool SaveInBinaryMode(const std::wstring& filePath, const char* data, int dataSize, bool isAppend = false);

		//创建目录，可以一次递归创建多级， 存在或者创建成功，返回true
		static bool CreateDirectories(const std::wstring& path);
		//删除文件或目录，
		static int RemoveAll(const std::wstring& path);
		static int RemoveAll(const std::wstring& path, const std::function<bool(const DirectoryEntry& entry)>& filter);

		// 复制文件或者目录
		static void Copy(const std::wstring& srcPath, const std::wstring& destPath);
		static void Copy(const std::wstring& srcPath, const std::wstring& destPath, const std::function<bool(const DirectoryEntry& entry)>& filter);

		//获取文件夹下所有指定类型的的文件(非递归)
		static std::vector<std::wstring> GetFiles(const std::wstring& path, const std::wstring& ext);

		//按过滤器获取文件夹下的的文件全路径(默认非递归)
		static std::vector<std::wstring> GetFilePathsByFilter(const std::wstring& path,
			std::function<bool(const DirectoryEntry& entry)> filter,
			bool recursively = false);

		static bool AppendHideAttribute(const std::wstring& filePath);
		static bool RemoveHideAttribute(const std::wstring& filePath);
	};
}
