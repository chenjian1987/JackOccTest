#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"
#include "OwnerPtr.h"

namespace JackC
{
    class IConfigRecord;

    //BehaviorConfig配置记录的名称
    struct JACKC_COMMON_INTERFACE_EXPORT BehaviorConfigRecordNames
    {
        static const wchar_t* ConfigFileVersion;          ///< 配置文件版本号
        static const wchar_t* EnableLog;                  ///< 是否启用日志
        static const wchar_t* EnableDump;                 ///< 是否启用Dump
        static const wchar_t* EnableDebugMessage;         ///< 是否启用调试信息
        static const wchar_t* ClientAppName;              ///< 应用名称
        static const wchar_t* ProductType;                ///< 产品许可类型
        static const wchar_t* ProductStage;               ///< 产品发布阶段
        static const wchar_t* RunAsService;               ///< 是否配置为服务
        static const wchar_t* CubeServiceHost;            ///< Cube服务Host
        static const wchar_t* UserCenterHost;             ///< 用户中心Host
    };



    //管理行为配置文件
    class JACKC_COMMON_INTERFACE_EXPORT IBehaviorConfig
    {
    public:
        //设置配置文件路径,并加配置项，第一次使用前，用户需指定behavior_config的路径
        static bool SetBehaviorConfigPath(const std::wstring& behaviorConfigPath);

        //通过配置项名称获取配置项
        static const IConfigRecord* GetConfigRecord(const std::wstring& recordName);

        //获取配置项的整型值
        static int GetConfigRecordValueAsInt(const std::wstring& recordName);

        //获取配置项的浮点型值
        static double GetConfigRecordValueAsDouble(const std::wstring& recordName);

        //获取配置项的字符串型值
        static std::wstring GetConfigRecordValueAsString(const std::wstring& recordName);

        //保存配置文件
        static bool SaveConfigFile();

        //是否为合法产品许可
        static bool IsValidProductType(const std::wstring& productType);

        //添加一条配置，如果同名配置已经存在，则跳过并返回false
        static bool AddConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);

        //修改一条配置，不存在则跳过并返回false
        static bool UpdateConfigRecord(OwnerPtr<IConfigRecord> opConfigRecord);

        //设置配置文件路径
        static std::wstring RedirectConfigFilePath(const std::wstring& configFilePath);

    };
}
