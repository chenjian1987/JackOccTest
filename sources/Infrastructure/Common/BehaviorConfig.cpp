#include <sstream>
#include <iomanip>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "pugixml.h"

#include "BehaviorConfig.h"
#include "FileSystem.h"
#include "TransferOwnership.h"

using namespace JackC;

//获取BehaviorConfigMgr单例
BehaviorConfigMgr* BehaviorConfigMgr::Get()
{
    static BehaviorConfigMgr s_instance;
    return &s_instance;
}

bool BehaviorConfigMgr::LoadConfigFile()
{
    pugi::xml_document xmlDoc;
    pugi::xml_parse_result result = xmlDoc.load_file(m_configFilePath.c_str());
    if (result.status != pugi::status_ok) return false;

    m_configRecord.clear();
    bool bSuccessed = true;
    pugi::xpath_node_set nodeSet = xmlDoc.select_nodes(L"ConfigRoot/ConfigRecord");

    for (auto pchild = nodeSet.begin(); pchild != nodeSet.end(); pchild++)
    {
        std::wstring attrType = pchild->node().attribute(L"type").as_string();
        std::wstring attrName = pchild->node().attribute(L"name").as_string();
        std::wstring attrValue = pchild->node().attribute(L"value").as_string();

        ConfigRecordType cfgRecordType = ConfigRecordType::InValid;
        //underlying_type:指出枚举的底层类型
        /*
            enum e1{};
            enum class e2:int {};

            bool e1_type=std::is_same<unsigned, typename  std::underlying_type<e1>::type> ::value; -------true
            bool e2_type=std::is_same<int, typename std::underlying_type<e2>::type>::value;        -------true
        */
        for (int loop = static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::InValid); loop < static_cast<std::underlying_type<ConfigRecordType>::type>(ConfigRecordType::Num_Of_Types); loop++)
        {
            if (IConfigRecord::getConfigRecordTypeNames()[loop] == attrType)
            {
                cfgRecordType = (ConfigRecordType)loop;
                break;
            }
        }
        switch (cfgRecordType)
        {
        case ConfigRecordType::Double:
            {
                std::wistringstream wiss(attrValue);
                double valueDouble = 0;
                wiss >> valueDouble;
                OwnerPtr<IConfigRecord> ownConfigRecord = IConfigRecord::create(attrName, valueDouble);
                AddConfigRecord(TransferOwnership(ownConfigRecord));
            }
            break;
        case ConfigRecordType::Int:
            {
                std::wistringstream wiss(attrValue);
                int valueDouble = 0;
                wiss >> valueDouble;
                OwnerPtr<IConfigRecord> ownConfigRecord = IConfigRecord::create(attrName, valueDouble);
                AddConfigRecord(TransferOwnership(ownConfigRecord));
            }
            break;
        case ConfigRecordType::String:
            {
                OwnerPtr<IConfigRecord> opConfigRecord = IConfigRecord::create(attrName, attrValue);
                AddConfigRecord(TransferOwnership(opConfigRecord));
            }
            break;
        default:
            {
                return false;
            }
        }
    }
    return bSuccessed;
}

//获取配置内容
const IConfigRecord* BehaviorConfigMgr::GetConfigRecord(const std::wstring& recordName) const
{
    for (int index = 0; index < (int)m_configRecord.size(); index++)
    {
        if (m_configRecord[index]->getName() == recordName)
        {
            return m_configRecord[index].get();
        }
    }
    return nullptr;
}
const std::vector<OwnerPtr<IConfigRecord>>& BehaviorConfigMgr::GetConfigRecords() const
{
    return m_configRecord;
}
bool BehaviorConfigMgr::IsValidProductType(const std::wstring& productType)
{
    return true;
}


bool BehaviorConfigMgr::AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    if (GetConfigRecord(opConfigRecord->getName())) return false;
    m_configRecord.push_back(TransferOwnership(opConfigRecord));
    return true;
}

bool BehaviorConfigMgr::SetBehaviorConfigPath(const std::wstring& behaviorConfigPath)
{
    FileOwnerPtr fp = FileSystem::OpenFile(behaviorConfigPath, L"rb");
    if (m_configFilePath == behaviorConfigPath) return true;

    m_configFilePath = behaviorConfigPath;
    return LoadConfigFile();
}

bool BehaviorConfigMgr::UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord)
{
    FOR_EACH(record, m_configRecord) {
        if (opConfigRecord->getName() == record->getName())
        {
            record = TransferOwnership(opConfigRecord);
            return true;
        }
    }
    return false;
}

std::wstring BehaviorConfigMgr::RedirectConfigFilePath(const std::wstring& configFilePath)
{
    std::wstring previousConfigFilePath = m_configFilePath;
    m_configFilePath = configFilePath;
    return previousConfigFilePath;
}

//保存配置文件
bool BehaviorConfigMgr::SaveConfigFile() const
{
    pugi::xml_document xmlDoc;
    xmlDoc.append_child(L"ConfigRoot");
    for (int index = 0; index < (int)m_configRecord.size(); index++)
    {
        const IConfigRecord* pRecord = m_configRecord[index].get();

        pugi::xml_node recordNode = xmlDoc.first_child().append_child(L"ConfigRecord");
        recordNode.append_attribute(L"type") = IConfigRecord::getConfigRecordTypeNames()[static_cast<std::underlying_type<ConfigRecordType>::type>(pRecord->getType())].c_str();
        recordNode.append_attribute(L"name") = pRecord->getName().c_str();
        recordNode.append_attribute(L"value") = pRecord->getStringOfValue().c_str();
    }
    return xmlDoc.save_file(m_configFilePath.c_str());
}