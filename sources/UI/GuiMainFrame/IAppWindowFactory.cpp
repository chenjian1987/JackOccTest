#include "IAppWindowFactory.h"
#include "DebugMessage.h"
#include "FileSystem.h"


using namespace JackC;



IAppWindowFactory* IAppWindowFactory::ms_pFactory = nullptr;

IAppWindowFactory* IAppWindowFactory::Get()
{
    if (ms_pFactory == nullptr)
    {
        FileSystem::ModuleHandle handle = FileSystem::LoadModule(L"CbimGuiMainFrame.dll");
        DBG_WARN_AND_RETURN_NULLPTR_UNLESS(handle, L"CbimGuiMainFrame.dll º”‘ÿ ß∞‹£°", L"jack.chen", L"2025-05-01");
    }

    DBG_WARN_AND_RETURN_NULLPTR_UNLESS(ms_pFactory, L"ms_pFactoryŒ™ø’", L"jack.chen", L"2025-05-01");
    return ms_pFactory;
}
