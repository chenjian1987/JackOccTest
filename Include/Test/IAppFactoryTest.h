#pragma once
#include "AppInterfaceTestDefine.h"

//class IApplication;

//平台用程序创建工厂--单例模式
class JACKC_APP_INTERFACE_TEST_EXPORT IAppFactoryTest
{
    /*protected:
        static IAppFactory* m_pFactory;
    public:
        static IAppFactory* Get();
        virtual ~IAppFactory(){}

        virtual void Destroy() = 0;

        virtual IApplication* CreateApp(int& argc, char** argv) = 0;
        virtual IApplication* GetApplication() = 0;*/

public:
    IAppFactoryTest() {};
    void test();
};