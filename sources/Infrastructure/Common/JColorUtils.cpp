#include "JColorUtils.h"
#include <regex>
#include "Color.h"
#include "StringUtils.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

std::wstring ColorUtils::GetStringOfColor(const Color& color)
{
    std::wstring ret;
    ret += StringUtils::ToWString((int)color.GetRed()) + L",";
    ret += StringUtils::ToWString((int)color.GetGreen()) + L",";
    ret += StringUtils::ToWString((int)color.GetBlue());
    return ret;
}

bool ColorUtils::GetColorFromString(const std::wstring& input, Color& color)
{
    std::regex  pattern("^((2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)(,|\\s+)){2}(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)$");

    std::string express = StringUtils::ToString(input);
    if (std::regex_match(express, pattern))
    {
        std::vector<int> colorNum;
        std::regex  rPattern("(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)");
        std::string::const_iterator iterStart = express.begin();
        std::string::const_iterator iterEnd = express.end();
        std::smatch result;
        while (std::regex_search(iterStart, iterEnd, result, rPattern))
        {
            colorNum.push_back(StringUtils::ToNumber<int>(result[0]));
            iterStart = result[0].second;
        }
        if (colorNum.size() == 3)
        {
            color.SetRed(colorNum[0]);
            color.SetGreen(colorNum[1]);
            color.SetBlue(colorNum[2]);
            return true;
        }
    }
    return false;
}

bool ColorUtils::CheckIsValidColor(const std::wstring& input, Color& color)
{
    std::wregex regExp(L"^([\\s]*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)[\\s]*,){2}[\\s]*(2[0-4][0-9]|25[0-5]|[01]?[0-9][0-9]?)[\\s]*,?[\\s]*(0\\.\\d{1,2}|1|0)?$",
        std::regex::icase);
    if (std::regex_match(input, regExp))
    {
        std::vector<std::wstring> colorRGB = StringUtils::splitString(input, L",", true);
        if (colorRGB.size() == 3)
        {
            color.SetRed(std::stoi(StringUtils::TrimSpace(colorRGB[0])));
            color.SetGreen(std::stoi(StringUtils::TrimSpace(colorRGB[1])));
            color.SetBlue(std::stoi(StringUtils::TrimSpace(colorRGB[2])));
            return true;
        }
    }
    return false;
}



