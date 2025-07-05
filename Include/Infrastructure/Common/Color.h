#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"
#include "JTypeDefs.h"

namespace JackC 
{
    //结构体Color用RGBA来表示颜色。
    ///
    /// 分别以R(red)、G(green)、B(blue)三个通道来进行描述，并以A(alpha)通道来标记透明度。
    /// 其中各通道范围: 
    ///     R: 0~255 
    ///     G: 0~255
    ///     B: 0~255
    ///     A: 0~255  其中'0'表示完全透明， '255'表示完全不透明
    struct JACKC_COMMON_INTERFACE_EXPORT Color
    {
        DEFINE_VISUAL_TYPE_UINT8(IntBaseType::Hex)

            unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;

        ///构造函数, 默认为黑色，完全不透明
        Color() : R(0), G(0), B(0), A(0xff)
        {}

        ///构造函数, 通过指定各颜色分量构造
        /// \param r  red分量
        /// \param g  green分量
        /// \param b  blue分量
        /// \param a  alpha分量, 默认完全不透明
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xff) :
            R(r), G(g), B(b), A(a)
        {}


        ///创建一个默认颜色属性
        ///返回一个默认的颜色属性，黑色不透明
        static Color Create() { return Color(); }

        ///通过指定R,G,B 三个通道的分量创建一个颜色属性，不透明
        static Color CreateByRGB(unsigned char r, unsigned char g, unsigned char b) { return Color(r, g, b); }


        ///设置颜色属性的 red 分量
        bool SetRed(unsigned char r) { R = r; return true; }

        ///设置颜色属性的 green 分量
        bool SetGreen(unsigned char g) { G = g; return true; }

        ///设置颜色属性的 blue 分量
        bool SetBlue(unsigned char b) { B = b; return true; }

        ///获取当前颜色的red分量
        unsigned char GetRed() const { return R; }

        ///获取当前颜色的green分量
        unsigned char GetGreen() const { return G; }

        ///获取当前颜色的blue分量
        unsigned char GetBlue() const { return B; }

        ///颜色色值比较。
        bool IsLessThan(const Color& other) const { return *this < other; }

        ///判断两个颜色属性是否相同。
        bool HasSameValue(const Color& other) const { return *this == other; }

        ///重载==操作符
        bool operator == (const Color& another) const
        {
            if (this == &another)
            {
                return true;
            }

            return (R == another.R) && (G == another.G) && (B == another.B) && (A == another.A);
        }

        ///重载<操作符
        bool operator < (const Color& another) const
        {
            if (R != another.R)
                return R < another.R;
            if (G != another.G)
                return G < another.G;
            if (B != another.B)
                return B < another.B;
            if (A != another.A)
                return A < another.A;
            return false;
        }

        ///重载!=操作符
        bool operator != (const Color& another) const
        {
            return !(*this == another);
        }

        ///获取SmartColor配色
        ///colorForLight 浅色背景下的SmartColor配色, 传出参数
        ///colorForDark  深色背景下的SmartColor配色, 传出参数
        static void GetSmartColor(Color& colorForLight, Color& colorForDark);

        /// 设置SmartColor配色
        ///
        /// 1） 默认为 浅色背景下为 DarkBackGround， 深色背景下为LightBackGround 可修改
        /// 2） 深浅背景以hsb颜色空间的B区进行区分， 亮度为51-100为浅背景， 0-50为深背景
        static void SetSmartColor(const Color& colorForLight, const Color& colorForDark);

        ///黑白定义
        ///
        /// White(255, 255, 255)
        /// Black(0, 0, 0) 
        static const Color White;
        static const Color Black;

        ///色光三原色
        ///
        /// Red(255, 0, 0)
        /// Green(0, 255, 0)
        /// Blue(0, 0, 255)
        static const Color Red;
        static const Color Green;
        static const Color Blue;

        ///颜料三原色的色光表示
        ///
        /// Yellow(255, 255, 0)
        /// Magenta(255, 0, 255)
        /// Cyan(0, 255, 255)
        static const Color Yellow;
        static const Color Magenta;
        static const Color Cyan;

        /// 其他颜色
        ///
        /// DarkSlateGray(47, 79, 79)
        /// Teal(0, 128, 128);
        /// PaleGodenrod(238, 232, 170);
        /// Orange(255, 165, 0);
        /// Sienna(160, 82, 45);
        /// FireBrick(178, 34, 34);
        /// Tan(210, 180, 140);
        /// SteelBlue(70, 130, 180);
        /// OliveDrab(85, 107, 47);
        /// Purple(128, 0, 128);
        static const Color DarkSlateGray;
        static const Color Teal;
        static const Color PaleGodenrod;
        static const Color Orange;
        static const Color Sienna;
        static const Color FireBrick;
        static const Color Tan;
        static const Color SteelBlue;
        static const Color OliveDrab;
        static const Color Purple;

        //灰度颜色定义
        /// WhiteSmoke(245, 245, 245);
        /// GainSboro(220, 220, 220);
        /// LightGray(211, 211, 211);
        /// Silver(192, 192, 192);
        /// DarkGray(169, 169, 169);
        /// Gray(128, 128, 128);
        /// DimGray(105, 105, 105);
        static const Color WhiteSmoke;
        static const Color GainSboro;
        static const Color LightGray;
        static const Color Silver;
        static const Color DarkGray;
        static const Color Gray;
        static const Color DimGray;

        ///背景颜色
        ///
        /// 深色背景(25, 26, 28) 
        /// 浅色背景(230, 229, 227)
        /// 
        ///  可自定义修改
        static Color DarkBackGround;
        static Color LightBackGround;
    };
}
