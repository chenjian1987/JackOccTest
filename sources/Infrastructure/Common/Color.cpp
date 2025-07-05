#include "Color.h"

using namespace JackC;

namespace
{
    static Color SmartColorForLightBackGround;
    static Color SmartColorForDarkBackGround;
    static Bool SmartColorIsSetted = false;
};
// 常用颜色定义

// 黑白定义
const Color Color::White(255, 255, 255);
const Color Color::Black(0, 0, 0);

// 色光三原色
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);

// 颜料三原色的色光表示
const Color Color::Yellow(255, 255, 0);
const Color Color::Magenta(255, 0, 255);
const Color Color::Cyan(0, 255, 255);

// 其它颜色定义
const Color Color::DarkSlateGray(47, 79, 79);
const Color Color::Teal(0, 128, 128);
const Color Color::PaleGodenrod(238, 232, 170);
const Color Color::Orange(255, 165, 0);
const Color Color::Sienna(160, 82, 45);
const Color Color::FireBrick(178, 34, 34);
const Color Color::Tan(210, 180, 140);
const Color Color::SteelBlue(70, 130, 180);
const Color Color::OliveDrab(85, 107, 47);
const Color Color::Purple(128, 0, 128);

// 灰度颜色定义
const Color Color::WhiteSmoke(245, 245, 245);
const Color Color::GainSboro(220, 220, 220);
const Color Color::LightGray(211, 211, 211);
const Color Color::Silver(192, 192, 192);
const Color Color::DarkGray(169, 169, 169);
const Color Color::Gray(128, 128, 128);
const Color Color::DimGray(105, 105, 105);

//深色背景(25, 26, 28)  浅色背景 (230, 229, 227)
Color Color::DarkBackGround(25, 26, 28);
Color Color::LightBackGround(230, 229, 227);

void Color::GetSmartColor(Color& colorForLight, Color& colorForDark)
{
    if (!SmartColorIsSetted)
    {
        //避免静态变量初始化顺序依赖
        SetSmartColor(DarkBackGround, LightBackGround);
    }
    colorForLight = SmartColorForLightBackGround;
    colorForDark = SmartColorForDarkBackGround;
}


void Color::SetSmartColor(const Color& colorForLight, const Color& colorForDark)
{
    SmartColorIsSetted = true;
    SmartColorForDarkBackGround = colorForDark;
    SmartColorForLightBackGround = colorForLight;
}