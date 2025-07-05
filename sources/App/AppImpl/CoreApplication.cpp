#include <Windows.h>

#include "CoreApplication.h"
#include "IApplication.h"
#include "DebugMessage.h"
#include "AppManager.h"
#include "IAppWindow.h"




using namespace JackC;



extern LONG WINAPI Filter(struct _EXCEPTION_POINTERS* exception);


CoreApplication::CoreApplication()
{
    IApplication* pIApp = IApplication::Get();
    DBG_WARN_AND_RETURN_VOID_UNLESS(pIApp, L"pApp is nullptr", L"jack.chen", L"2025-05-01");
    QApplication* pQApp = dynamic_cast<QApplication*>(pIApp);
    pQApp->installEventFilter(this);            //应用程序的主事件循环，负责调度和处理所有事件
}
CoreApplication::~CoreApplication()
{

}










int CoreApplication::Execute()
{
    IApplication* pApplication = IApplication::Get();
    DBG_WARN_AND_RETURN_UNLESS(pApplication, -1, L"pApplication is null", L"jack.chen", L"2025-05-01");

    IAppWindow* pAppWindow = pApplication->GetAppWindow();
    DBG_WARN_AND_RETURN_UNLESS(pAppWindow, -1, L"pAppWindow is null", L"jack.chen", L"2025-05-01");

    pAppWindow->Initialize();

    // 注意：强制执行当前事件队列，确保有请求退出程序请求被执行
    QApplication::processEvents();

    return  CoreApplication::exec();
}


int ExecWithHandledException()
{
#ifdef __linux__
    try
    {
        return QApplication::exec();
    }
    catch (...)
    {

    }
#else
    __try
    {
        return QApplication::exec();
    }
    __except (Filter(GetExceptionInformation()))
    {

    }
#endif
    return 1;
}
int CoreApplication::exec()
{
    bool isEnableHandledException = true;
    if (isEnableHandledException)
    {
        return ExecWithHandledException();
    }
    else
    {
        return QApplication::exec();
    }
}