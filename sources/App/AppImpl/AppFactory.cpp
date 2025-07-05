#include "AppFactory.h"
#include "JNew.h"
#include "DebugMessage.h"
#include "Application.h"

using namespace JackC;



AppFactory s_factory;


AppFactory::AppFactory()
{
    IAppFactory::m_pFactory = this;
}
AppFactory::~AppFactory()
{
    IAppFactory::m_pFactory = nullptr;
}



IApplication* AppFactory::GetApplication()
{
    return m_opApplication.get();
}


IApplication* AppFactory::CreateApp(int& argc, char** argv)
{
    DBG_WARN_AND_RETURN_UNLESS(!m_opApplication, m_opApplication.get(), L"Application is created", L"jack.chen", l"2025-05-01");
    m_opApplication = NEW_AS_OWNER_PTR(Application, argc, argv);
    return m_opApplication.get();
}
