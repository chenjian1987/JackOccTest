#pragma once
#include "IAppWindowFactory.h"


namespace JackC
{

    class MainWindowFactory : public IAppWindowFactory
    {
        CANNOT_COPY_OR_ASSIGN(MainWindowFactory);
    public:
        MainWindowFactory();
        virtual ~MainWindowFactory() final;

        virtual OwnerPtr<IAppWindow> CreateAppWindow() final;


    };

}
