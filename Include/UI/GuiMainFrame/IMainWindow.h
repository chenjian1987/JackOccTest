
#pragma once
#include "GuiMainFrameDefine.h"
#include "IAppWindow.h"
#include "OwnerPtr.h"
#include "IApplication.h"





namespace JackC
{
    class QObject;

    class JACKC_GUI_MAIN_FRAME_EXPORT IMainWindow:public IAppWindow
    {
    public:
        virtual ~IMainWindow();

    public:
        static IMainWindow* GetMainWindow();
        //static QWidget* GetMainWindowWidget();

        static OwnerPtr<IAppWindow> Create();
    };

}