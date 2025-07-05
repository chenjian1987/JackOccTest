#include "IBehaviorConfig.h"
#include "IConfigRecord.h"
#include "BehaviorConfig.h"
#include "TransferOwnership.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

// BehaviorConfigÅäÖÃ¼ÇÂ¼µÄÃû³Æ
const wchar_t* BehaviorConfigRecordNames::ConfigFileVersion = L"ConfigFileVersion";
const wchar_t* BehaviorConfigRecordNames::EnableLog = L"EnableLog";
const wchar_t* BehaviorConfigRecordNames::EnableDump = L"EnableDump";
const wchar_t* BehaviorConfigRecordNames::EnableDebugMessage = L"EnableDebugMessage";
const wchar_t* BehaviorConfigRecordNames::ClientAppName = L"ClientAppName";
const wchar_t* BehaviorConfigRecordNames::ProductType = L"ProductType";
const wchar_t* BehaviorConfigRecordNames::ProductStage = L"ProductStage";
const wchar_t* BehaviorConfigRecordNames::RunAsService = L"RunAsService";
const wchar_t* BehaviorConfigRecordNames::CubeServiceHost = L"CubeServiceHost";
const wchar_t* BehaviorConfigRecordNames::UserCenterHost = L"UserCenterHost";


bool IBehaviorConfig::SetBehaviorConfigPath(const std::wstring& behaviorConfigPath)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->SetBehaviorConfigPath(behaviorConfigPath);
    }
    return false;
}

int IBehaviorConfig::GetConfigRecordValueAsInt(const std::wstring& recordName)
{
    if (GetConfigRecord(recordName) != nullptr)
    {
        return GetConfigRecord(recordName)->getValueAsInt();
    }
    return 0;
}

double IBehaviorConfig::GetConfigRecordValueAsDouble(const std::wstring& recordName)
{
    if (GetConfigRecord(recordName) != nullptr)
    {
        return GetConfigRecord(recordName)->getValueAsDouble();
    }
    return 0.0;
}

std::wstring IBehaviorConfig::GetConfigRecordValueAsString(const std::wstring& recordName)
{
    if (GetConfigRecord(recordName) != nullptr)
    {
        return GetConfigRecord(recordName)->getValueAsString();
    }
    return L"";
}

const IConfigRecord* IBehaviorConfig::GetConfigRecord(const std::wstring& recordName)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->GetConfigRecord(recordName);
    }
    return nullptr;
}

bool IBehaviorConfig::AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->AddConfigRecord(TransferOwnership(opConfigRecord));
    }
    return false;
}

bool IBehaviorConfig::IsValidProductType(const std::wstring& productType)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->IsValidProductType(productType);
    }
    return false;
}

bool IBehaviorConfig::UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->UpdateConfigRecord(TransferOwnership(opConfigRecord));
    }
    return false;
}

std::wstring IBehaviorConfig::RedirectConfigFilePath(const std::wstring& configFilePath)
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->RedirectConfigFilePath(configFilePath);
    }
    return L"";
}

bool IBehaviorConfig::SaveConfigFile()
{
    if (BehaviorConfigMgr::Get() != nullptr)
    {
        return BehaviorConfigMgr::Get()->SaveConfigFile();
    }
    return false;
}
