#pragma once
#include "UiViewDefine.h"
#include "OwnerPtr.h"




namespace JackC
{
    class IAppWindow;



    class JACKC_UIVIEW_EXPORT UiManager 
    {
    public:
        UiManager();
        virtual ~UiManager() {};

        static UiManager* GetInstance();
        

    public:
        virtual IAppWindow* GetApplicationWindow();
        virtual bool SetApplicationWindow(IAppWindow* pWindow);

    private:
        IAppWindow* m_pWindow;
     
    };

}

