#include "IConfigRecord.h"
#include "ConfigRecord.h"
#include "JNew.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

OwnerPtr<IConfigRecord> IConfigRecord::create(const std::wstring& name, const std::wstring& stringValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::String, name, stringValue);
}

OwnerPtr<IConfigRecord> IConfigRecord::create(const std::wstring& name, double doubleValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::Double, name, doubleValue);
}

OwnerPtr<IConfigRecord> IConfigRecord::create(const std::wstring& name, int intValue)
{
    return NEW_AS_OWNER_PTR(ConfigRecord, ConfigRecordType::Int, name, intValue);
}

//static_const：1. 一般用于值类型的转换（由窄转宽） 2.void指针和具体类型指针之间的转换。 3.枚举转换
const std::vector<std::wstring>& IConfigRecord::getConfigRecordTypeNames()
{
    //underlying_type:获取枚举类型的基础类型 s_configNames(int)
    static std::vector<std::wstring> s_configNames(static_cast<std::underlying_type<ConfigRecordType>::type> (ConfigRecordType::Num_Of_Types));

    s_configNames[static_cast<std::underlying_type<ConfigRecordType>::type> (ConfigRecordType::InValid)] = L"invalid";
    s_configNames[static_cast<std::underlying_type<ConfigRecordType>::type> (ConfigRecordType::String)] = L"string";
    s_configNames[static_cast<std::underlying_type<ConfigRecordType>::type> (ConfigRecordType::Int)] = L"int";
    s_configNames[static_cast<std::underlying_type<ConfigRecordType>::type> (ConfigRecordType::Double)] = L"double";

    return s_configNames;
}

