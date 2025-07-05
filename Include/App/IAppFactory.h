#pragma once

#include "AppInterfaceDefine.h"


namespace JackC
{
    class IApplication;

    class JACKC_APP_INTERFACE_EXPORT IAppFactory
    {
    protected:
        static IAppFactory* m_pFactory;

    public:
        static IAppFactory* Get();

        virtual ~IAppFactory() {}

    public:
       /* virtual void Destroy() = 0;*/
        virtual IApplication* CreateApp(int& argc, char** argv) = 0;

        virtual IApplication* GetApplication() = 0;

    };
}