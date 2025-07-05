#pragma once

#include "AppInterfaceDefine.h"
#include "IAppFactory.h"


namespace JackC
{
    class MainWindow;
    class IAppWindow;



    class JACKC_APP_INTERFACE_EXPORT IApplication
    {
    protected:
        static IApplication* m_pFactory;

    public:
        static IApplication* Get();

        virtual ~IApplication();

    public:
        virtual int Execute() = 0;
        virtual IAppWindow* GetAppWindow() const = 0;
    };
}