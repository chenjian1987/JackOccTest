#pragma once
#include "AppImplDefine.h"
#include "IAppFactory.h"
#include "OwnerPtr.h"



namespace JackC
{
    class IApplication;

    class JACKC_APP_IMPL_EXPORT AppFactory:public IAppFactory
    {
        CANNOT_COPY_OR_ASSIGN(AppFactory);

    public:
        AppFactory();
        virtual ~AppFactory() final;

    public:
        /*virtual void Destroy() final;*/
        virtual IApplication* CreateApp(int& argc, char** argv) final;
        virtual IApplication* GetApplication() final;

    private:
        OwnerPtr<IApplication> m_opApplication;

    };


}