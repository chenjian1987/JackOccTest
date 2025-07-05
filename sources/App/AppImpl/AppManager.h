#pragma once
#include "AppImplDefine.h"


namespace JackC
{
    class IApplication;

    class JACKC_APP_IMPL_EXPORT AppManager
    {
    public:
        static IApplication* Create(int& argc, char** argv);

        static void Destroy();


    };

}