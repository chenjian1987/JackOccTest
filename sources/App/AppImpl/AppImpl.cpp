#include "AppImpl.h"
#include "UiManager.h"
#include "CoreApplication.h"





using namespace JackC;




AppImpl::AppImpl(IApplication* pApp)
{

}
AppImpl::~AppImpl()
{

}



int AppImpl::Execute()
{
    CoreApplication pCoreApp;
    pCoreApp.Execute();


    return 1;
}



IAppWindow* AppImpl::GetMainWindow() const
{
    UiManager* pUiMgr = UiManager::GetInstance();
    return pUiMgr->GetApplicationWindow();
}