#include "UiManager.h"


using namespace JackC;





UiManager::UiManager() 
{
}

UiManager* UiManager::GetInstance()
{
    static UiManager sInstance;
    return &sInstance;
}






IAppWindow* UiManager::GetApplicationWindow()
{
    return m_pWindow;
}

bool UiManager::SetApplicationWindow(IAppWindow* pWindow)
{
    m_pWindow = pWindow;
    return true;
}
