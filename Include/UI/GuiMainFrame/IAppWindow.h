#pragma once
#include "GuiMainFrameDefine.h"




namespace JackC
{

    class JACKC_GUI_MAIN_FRAME_EXPORT IAppWindow
    {
    public:
        IAppWindow(){}
        virtual ~IAppWindow(){}



    public:
        virtual bool Initialize() = 0;
        
    };



}