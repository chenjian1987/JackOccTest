#include <iostream>
#include "JString.h"
#include "StringUtils.h"

#include "StringTest.h"
#include <codecvt>
#include <locale>
#include <Windows.h>
#include "FilePath.h"
#include "BehaviorConfig.h"
#include "JNew.h"



//
using namespace JackC;

void StringTest::Test1()
{
    const std::wstring src = L"Jack.chen";   //字符串前面加L表示该字符串是Unicode字符串，就是每个字符占两个字节
    //_T是一个宏，如果项目使用了Unicode字符集，定义了UNICODE宏，则自动在字符串的前面加L，否则字符串不变，因此一般在C+=，用_T来保证兼容性

    std::string str2 = StringUtils::ToString(src);
}


void StringTest::filePathTest1()
{
    std::wstring path = L"F:/cube-desktop/cube-desktop.sln";
    FilePath filePath(path);
}


void StringTest::behaviorConfigTest()
{
    //OwnerPtr<BehaviorConfigMgr> mgr= NEW_AS_OWNER_PTR(BehaviorConfigMgr) ;
    //BehaviorConfigMgr::Get()->SaveConfigFile();

    //BehaviorConfigMgr::SaveConfigFile();

    // CubeGuid::CubeGuid(L"3E3916C8-9B89-4D05-893D-3A6C1A2418F7"),

}

void StringTest::guidTest()
{
    JGuid guid(L"3E3916C8-9B89-4D05-893D-3A6C1A2418F7");
    std::wstring wstring= guid.ToWString();
}

void StringTest::guidTest2(const JGuid guid)
{
    std::wstring str = guid.ToWString();

}