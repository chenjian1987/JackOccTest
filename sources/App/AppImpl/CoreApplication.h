#pragma once
#include <QtWidgets/qapplication.h>
#include <qobject.h>

#include "AppImplDefine.h"
#include "OwnerPtr.h"




namespace JackC
{

    class JACKC_APP_IMPL_EXPORT CoreApplication:public QObject
    {
    public:
        explicit CoreApplication();
        virtual ~CoreApplication();

    public:
        virtual int Execute();

        static int exec();




    };
}