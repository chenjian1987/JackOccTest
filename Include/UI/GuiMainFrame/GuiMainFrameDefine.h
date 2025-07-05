#pragma once
#include "JDefines.h"

#ifdef JACKC_GUI_MAIN_FRAME_HOME
#define JACKC_GUI_MAIN_FRAME_EXPORT DLL_EXPORT
#else 
#define JACKC_GUI_MAIN_FRAME_EXPORT DLL_IMPORT
#endif


enum class EnumCoreViewControlType
{
    Front,   // 正视
    Back,    // 后视
    Left,    // 左视
    Right,   // 右视
    Top,     // 俯视
    Bottom,  // 仰视
    Iso      // 等轴测
};
