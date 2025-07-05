#pragma once
#include "JCommonDefine.h"

#include <windows.h>


namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT KeyboardMouseStatesMgr
    {
        CANNOT_COPY_OR_ASSIGN(KeyboardMouseStatesMgr)
    private:
        KeyboardMouseStatesMgr();

    public:
        virtual ~KeyboardMouseStatesMgr(void);

    public:
        static KeyboardMouseStatesMgr* GetInstance();

        // ²éÑ¯Êó±ê°´¼ü×´Ì¬
        bool IsLeftButtonPressed() const;
        bool IsRightButtonPressed() const;
        bool IsMiddleButtonPressed() const;

        // ²éÑ¯¼üÅÌ°´¼ü×´Ì¬
        bool IsCtrlKeyPressed() const;
        bool IsShiftKeyPressed() const;
        bool IsAltKeyPressed() const;

        bool IsKeyAndButtonPressed(int code) const;
    };

}