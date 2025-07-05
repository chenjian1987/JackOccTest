#ifdef ENABLE_VLD_CHECK
#include "vld.h"
#endif

#include "AppManager.h"
#include "IApplication.h"
#include "IMainWindow.h"
#include <qcoreapplication.h>



using namespace JackC;



int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    IApplication* pApp = AppManager::Create(argc, argv);

    // 程序主窗口，main函数维护生命周期
    OwnerPtr<IAppWindow> opMainWindow = IMainWindow::Create();
    
    int result = pApp->Execute();

    // 在程序销毁前，销毁主窗口
    opMainWindow.reset();

    AppManager::Destroy();
    return result;
}
