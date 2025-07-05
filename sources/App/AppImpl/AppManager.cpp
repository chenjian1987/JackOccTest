
#pragma warning(disable: 4091)
#include <sstream>
#include <string>
#include <iostream>
#include <exception>
#include <Windows.h>
#pragma comment(lib, "Dbghelp.lib" )
#include <minidumpapiset.h>


#include "AppManager.h"
#include "AppImpl.h"
#include "IAppFactory.h"
#include "DebugMessage.h"
#include "Application.h"
#include "DebugMessage.h"
#include "LogManager.h"



using namespace JackC;


LONG WINAPI Filter(struct _EXCEPTION_POINTERS* exception)
{
    PEXCEPTION_RECORD pRecord = exception->ExceptionRecord; 
    std::wostringstream errorMsgStream;

    std::wstring dmpFileName = LogManager::Get()->GetLogsFolder() + L"\\DumpFile.dmp";
    errorMsgStream << L"程序遇到致命错误,错误信息如下:" << L"\n";
    errorMsgStream << L"异常产生原因:" << pRecord->ExceptionCode << L"\n";
    errorMsgStream << L"   异常标志:" << pRecord->ExceptionFlags << L"\n";
    errorMsgStream << L"崩溃文件位置:" << dmpFileName;
    FYI(L"应用程序产生异常!    " + errorMsgStream.str());

    HANDLE lhDumpFile = CreateFile(dmpFileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
    loExceptionInfo.ExceptionPointers = exception;
    loExceptionInfo.ThreadId = GetCurrentThreadId();
    loExceptionInfo.ClientPointers = TRUE;
    //当程序遇到严重错误时，可以利用MiniDumpWriteDump生成一个小型转储文件，文件包含内存等信息，开发人员可以使用调试工具调试这些信息。
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
    CloseHandle(lhDumpFile);

    DBG_WARN(errorMsgStream.str(), L"Jack.chen", L"2022-10-26");
    LogManager::Get()->Restart();

    return EXCEPTION_EXECUTE_HANDLER;
}



void PurecallHandler()
{
    std::wostringstream ss;
    ss << L"异常产生原因: 纯虚函数调用!";
    FYI(L"应用程序产生异常!    " + ss.str());
}


void EnableHandledException()
{
    //_WRITE_ABORT_MSG 控制程序调用abort函数终止时，是否要想标准错误流输出错误消息.  参数为0表示禁用
    _set_abort_behavior(0, _WRITE_ABORT_MSG);       //设置程序在调用abort函数时 终止行为
    //_CALL_REPORTFAULT  作用是程序在调用abort函数时，触发windows错误报告机智。
    _set_abort_behavior(0, _CALL_REPORTFAULT);
    // 用于设置纯虚函数调用处理程序
    _set_purecall_handler(PurecallHandler);
    SetUnhandledExceptionFilter(Filter);
}





IApplication* AppManager::Create(int& argc, char** argv)
{
    EnableHandledException();

    QStringList arguments;
    for (int i = 0; i < argc; i++)
    {
        arguments.push_back(QString::fromLocal8Bit(argv[i]));
    }

    IApplication* pIApp = IAppFactory::Get()->CreateApp(argc, argv);
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pIApp, L"pApp null");

    Application* pApp = dynamic_cast<Application*>(pIApp);
    return pIApp;
}



void AppManager::Destroy()
{

}