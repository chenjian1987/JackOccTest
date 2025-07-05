#include "DebugMessage.h"
#include "UiManager.h"
#include "AppWindowFactory.h"
#include "JNew.h"
#include "MainWindow.h"


namespace JackC
{
    MainWindowFactory s_MainWindowFactory;

    MainWindowFactory::MainWindowFactory()
    {
        ms_pFactory = this;
    }

    MainWindowFactory::~MainWindowFactory()
    {
        ms_pFactory = nullptr;
    }

    OwnerPtr<IAppWindow> MainWindowFactory::CreateAppWindow()
    {
        return NEW_AS_OWNER_PTR(MainWindow);
    }


}
