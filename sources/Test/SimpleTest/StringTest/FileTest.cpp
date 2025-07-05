#include <vector>

#include "FileTest.h"
#include "FilePathManager.h"
#include "FIleSystem.h"

using namespace JackC;

void FileTest::LoadResourceFiles()
{
    FilePathManager* pFilePathManager = FilePathManager::Get();
    //获取资源文件夹路径
    const FilePath& resourcePath = pFilePathManager->GetResourcePath();
    //获取资源文件夹的全路径
    FileInfo fileDirInfo(resourcePath.GetFullPath());

    //获取配置文件的具体路径
    std::wstring filePath(resourcePath.GetFullPath()+ L"//GuideFilesConfig.conf");

    FileInfo fileInfo(filePath);
    //获取文件内内容
    std::vector<char> fileContent = FileUtils::Load(filePath);

    std::string fileContentStr;
    //在str的开始处插入 begin 到end的所有字符
    fileContentStr.insert(fileContentStr.begin(), fileContent.begin(), fileContent.end());

    //内容分割
    std::vector<std::string> fileList = StringUtils::splitString(fileContentStr, "\n");





    return;



}