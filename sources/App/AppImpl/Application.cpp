#include "Application.h"
#include "JNew.h"
#include "AppImpl.h"



using namespace JackC;



Application::Application(int& argc, char** argv):QApplication(argc,argv)
{
    m_opAppImpl = NEW_AS_OWNER_PTR(AppImpl, this);

}
Application::~Application()
{ }



int Application::Execute()
{
    return m_opAppImpl->Execute();
}



IAppWindow* Application::GetAppWindow() const
{
    return m_opAppImpl->GetMainWindow();
}