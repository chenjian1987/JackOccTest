#pragma once
#include "AppImplDefine.h"
#include "IApplication.h"
#include "OwnerPtr.h"

#include <QApplication>
#include <QObject>




namespace JackC
{
    class AppImpl;

    class  Application:public QApplication,public IApplication
    {
        Q_OBJECT
        CANNOT_COPY_OR_ASSIGN(Application)
    public:
        virtual ~Application() final;

    private:
        Application(int& argc, char** argv);

    protected:
        int Execute() final;
        virtual IAppWindow* GetAppWindow() const override;

    private:
        OwnerPtr<AppImpl> m_opAppImpl;

        friend class AppFactory;
        friend class AppManager;
    };
}