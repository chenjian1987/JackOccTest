#pragma once

#include "JDevService.h"
#include "OwnerPtr.h"
#include "IConfigRecord.h"

namespace JackC
{
    class  BehaviorConfigMgr
    {
    private:
        std::vector<OwnerPtr<IConfigRecord>> m_configRecord;
        std::wstring m_configFilePath;

    private:
        const std::vector<OwnerPtr<IConfigRecord>>& GetConfigRecords() const;
        bool LoadConfigFile();
    public:
        static BehaviorConfigMgr* Get();

        //第一次使用前，用户需指定behavior_config的路径
        bool SetBehaviorConfigPath(const std::wstring& behaviorConfigPath);

        const IConfigRecord* GetConfigRecord(const std::wstring& recordName) const;

        bool SaveConfigFile() const;
        static bool IsValidProductType(const std::wstring& productType);

        //添加一条配置，如果同名则跳过
        bool AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);


        // 修改一条配置，不存在则跳过（可提供API）
        bool UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);

        //日志模块需要用到
        std::wstring RedirectConfigFilePath(const std::wstring& configFilePath);

    };
}

#define FIND_BEHAVIOR_CONFIG(name) JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)

#define LOOPUP_BEHAVIOR_CONFIG_INT_VALUE(name) \
    JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)==nullptr?0:JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->getValueAsInt())

#define LOOKUP_BEHAVIOR_CONFIG_DOUBLE_VALUE(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name) == nullptr ? 0.0 : JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->GetValueAsDouble())

#define LOOKUP_BEHAVIOR_CONFIG_STRING_VALUE(name) \
    (JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name) == nullptr ? L"" : JackC::BehaviorConfigMgr::Get()->GetConfigRecord(name)->GetValueAsString())
