#pragma once
#include "JDevService.h"
#include "Color.h"
#include "JCommonDefine.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT ColorUtils
    {
    public:

        //Color转为字符串（格式为 "R,G,B")
        static std::wstring GetStringOfColor(const Color& color);

        //从字符串转为Color
        static bool GetColorFromString(const std::wstring& input, Color& color);

        //新UI框架使用,从字符串取出颜色值并验证颜色值是否合法
        static bool CheckIsValidColor(const std::wstring& input, Color& color);
    };
}
