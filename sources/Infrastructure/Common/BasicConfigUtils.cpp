#include "BasicConfigUtils.h"
#include "IConfigRecord.h"
#include "IBehaviorConfig.h"

using namespace JackC;

namespace
{
    const int c_TargetIntValue = 1;
    const std::wstring c_TargetInHouseValue = L"8A6B3148-68DB-41E0-96DC-7BBD52209E7F";
};


bool BasicConfigUtils::DebugMessageEnabled()
{
    //log ´¦Àí

    if (IsAppInHouse())
    {
        return true;
    }
    return CompareConfigValue(BehaviorConfigRecordNames::EnableDebugMessage, c_TargetIntValue);
}

bool BasicConfigUtils::IsRunAsService()
{
    return CompareConfigValue(BehaviorConfigRecordNames::RunAsService, c_TargetIntValue);
}
bool BasicConfigUtils::LogEnabled()
{
    return CompareConfigValue(BehaviorConfigRecordNames::EnableLog, c_TargetIntValue);
}

bool BasicConfigUtils::DumpEnabled()
{
    return CompareConfigValue(BehaviorConfigRecordNames::EnableDump, c_TargetIntValue);
}

bool BasicConfigUtils::IsAppInHouse()
{
    return CompareConfigValue(BehaviorConfigRecordNames::ProductStage, c_TargetInHouseValue);
}


bool BasicConfigUtils::CompareConfigValue(const std::wstring &key, int target)
{
    const IConfigRecord* pConfigRecord = IBehaviorConfig::GetConfigRecord(key);
    if (pConfigRecord != nullptr)
    {
        return pConfigRecord->getValueAsInt() == target;
    }
    return false;
}
bool BasicConfigUtils::CompareConfigValue(const std::wstring& key, const std::wstring &target)
{
    const IConfigRecord* pConfigRecord = IBehaviorConfig::GetConfigRecord(key);
    if (pConfigRecord != nullptr)
    {
        return pConfigRecord->getStringOfValue()== target;
    }
    return false;
}