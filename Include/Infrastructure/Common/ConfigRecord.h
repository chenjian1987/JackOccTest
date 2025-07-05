#pragma once
#include "JCommonDefine.h"
#include "IConfigRecord.h"


#pragma warning(push)
#pragma warning(disable:4251)

namespace JackC
{
	//配置文件的一条记录，由类型，名称和值三项组成
	class JACKC_COMMON_INTERFACE_EXPORT ConfigRecord :public IConfigRecord
	{
	private:
		//配置项记录的功能开关 配置项的内部标识码
		static const int internalIdentifyCode;
	private:
		ConfigRecordType m_type;
		std::wstring m_name;
		std::wstring m_valueString;
		int m_valueInt;
		double m_valueDouble;
	public:
		ConfigRecord(ConfigRecordType type, const std::wstring& name, const std::wstring& stringValue);
		ConfigRecord(ConfigRecordType type, const std::wstring& name, int intValue);
		ConfigRecord(ConfigRecordType type, const std::wstring& name, double doubleValue);
		virtual ~ConfigRecord();

		ConfigRecord(const ConfigRecord& other);
		ConfigRecord& operator=(const ConfigRecord& other);

	public:
		virtual ConfigRecordType getType() const override { return m_type; }
		virtual std::wstring getName() const override { return m_name; }
	public:
		//配置项 记录 是否为内部标识码
		virtual bool IsInternalIdentifyCodeValue() const override;
		virtual OwnerPtr<IConfigRecord> clone() const override;
		virtual std::wstring getValueAsString() const override;
		virtual int getValueAsInt() const override;
		virtual double getValueAsDouble() const override;
		// 仅用于配置XML文件保存时
		virtual std::wstring getStringOfValue() const override;



	};
}
#pragma warning(pop)