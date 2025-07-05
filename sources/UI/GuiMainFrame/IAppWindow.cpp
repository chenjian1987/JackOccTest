#include "GuiMainFrameDefine.h"
#include "IAppWindow.h"
#include "DebugMessage.h"
#include "IAppWindowFactory.h"



using namespace JackC;





OwnerPtr<IAppWindow> IAppWindow::Create()
{
    IApplication* pIApp = IApplication::Get();

    IAppWindow* pIAppWindow = pIApp->GetAppWindow();
    DBG_WARN_AND_RETURN_UNLESS(pIAppWindow == nullptr, nullptr, L"created pIAppWindow again", L"jack.chen", L"2025-05-10");

    IAppWindowFactory* pAppWindowFactory = IAppWindowFactory::Get();
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pAppWindowFactory, L"pAppWindowFactory is nullptr", L"jack.chen", L"2025-05-10");

    return pAppWindowFactory->CreateAppWindow();
}


//QWidget* IMainWindow::GetMainWindowWidget()
//{
//    IGuiMainFrameFactory* pGuiMainframeFactory = IGuiMainFrameFactory::Get();
//    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pGuiMainframeFactory, L"pGuiMainframeFactoryÎª¿Õ", L"jack.chen", L"2025-05-10");
//    return pGuiMainframeFactory->GetMainWindowWidget();
//}

IAppWindow* IAppWindow::GetMainWindow()
{
    IApplication* pApp = IApplication::Get();
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS( pApp, L"pApp is nullptr", L"jack.chen", L"2025-05-10");

    IAppWindow* pAppWindow = pApp->GetAppWindow();
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pAppWindow, L"pAppWindowÎª¿Õ", L"jack.chen", L"2025-05-10");

    IAppWindow* pMainWindow = dynamic_cast<IAppWindow*>(pAppWindow);
    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pMainWindow, L"pMainWindowÎª¿Õ", L"jack.chen", L"2025-05-10");

    return pMainWindow;
}
