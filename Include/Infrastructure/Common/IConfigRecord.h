#pragma once
#include "JDevService.h"
#include "JDefines.h"
#include "JCommonDefine.h"
#include "OwnerPtr.h"

namespace JackC
{
	//所有支持的配置文件中的配置项纪录的类型
	enum class ConfigRecordType :int32_t
	{
		InValid = 0,   ///无效类型
		String,      //字符串类型
		Int,         //整数类型
		Double,      //双精度浮点数
		Num_Of_Types //支持的类型总数
	};

	//配置文件的一条配置项纪录
	FINAL_INTERFACE class JACKC_COMMON_INTERFACE_EXPORT IConfigRecord
	{
	public:
		//获取支持的配置项记录的类型名
		static const std::vector<std::wstring>& getConfigRecordTypeNames();
	public:
		//通过名字和字符串类型值 创建配置项记录
		static OwnerPtr<IConfigRecord> create(const std::wstring& name, const std::wstring& stringValue);
		//通过名字和整型类型值 创建配置项记录
		static OwnerPtr<IConfigRecord> create(const std::wstring& name, int intValue);
		//通过名字和双精度浮点类型值 创建配置项记录
		static OwnerPtr<IConfigRecord> create(const std::wstring& name, double doubleValue);
	public:
		//克隆一份配置项记录
		virtual OwnerPtr<IConfigRecord> clone() const = 0;

	public:
		//获取配置项记录的类型
		virtual ConfigRecordType getType() const = 0;
		//获取配置项记录的名称
		virtual std::wstring getName() const = 0;
		//获取配置项记录的字符串类型值
		virtual std::wstring getValueAsString() const = 0;
		//获取配置项记录的整型类型值
		virtual int getValueAsInt() const = 0;
		//获取配置项记录的 双精度浮点 类型值
		virtual double getValueAsDouble() const = 0;

		//获取配置项记录的任何类型值的字符串
		virtual std::wstring getStringOfValue() const = 0;

	public:
		//配置项 记录 是否为内部标识码
		virtual bool IsInternalIdentifyCodeValue() const = 0;
	};

}