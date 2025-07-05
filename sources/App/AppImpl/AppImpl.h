#pragma once
#include "AppImplDefine.h"


#include <QStringList>
#include <QTranslator>




namespace JackC
{
    class IApplication;
    class AppManager;
    class IAppWindow;

    class JACKC_APP_IMPL_EXPORT AppImpl
    {
        CANNOT_COPY_OR_ASSIGN(AppImpl)
    public:
        explicit AppImpl(IApplication* pApp);
        virtual ~AppImpl();

    public:
        int Execute();
        IAppWindow* GetMainWindow() const;


    private:
        friend class AppManager;


    };
}