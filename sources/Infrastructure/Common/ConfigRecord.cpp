#include "ConfigRecord.h"
#include "JNew.h"
#include <iostream>
#include <sstream>


#include"EnableCompileWarning_The_LAST_IncludeInCpp.h"
using namespace JackC;


const int ConfigRecord::internalIdentifyCode=-8360517;

ConfigRecord::~ConfigRecord(){}

ConfigRecord::ConfigRecord(ConfigRecordType type,const std::wstring& name, const std::wstring& stringValue)
	:m_type(type),m_name(name),m_valueString(stringValue),m_valueInt(0),m_valueDouble(0)
{
}
ConfigRecord::ConfigRecord(ConfigRecordType type,const std::wstring& name, int intValue)
	:m_type(type),m_name(name),m_valueString(),m_valueInt(intValue),m_valueDouble(0)
{
}
ConfigRecord::ConfigRecord(ConfigRecordType type,const std::wstring& name, double doubleValue)
	:m_type(type),m_name(name),m_valueString(),m_valueInt(0),m_valueDouble(doubleValue)
{
}

ConfigRecord::ConfigRecord(const ConfigRecord& other)
	:m_type(other.m_type),m_name(other.m_name),m_valueString(other.m_valueString),m_valueInt(other.m_valueInt),m_valueDouble(other.m_valueDouble)
{
}

ConfigRecord& ConfigRecord::operator=(const ConfigRecord& other)
{
	if(this==&other) return *this;
	m_type=other.m_type;
	m_name=other.m_name;
	m_valueString=other.m_valueString;
	m_valueInt=other.m_valueInt;
	m_valueDouble=other.m_valueDouble;
	return *this;
}

OwnerPtr<IConfigRecord> ConfigRecord::clone() const
{
	return NEW_AS_OWNER_PTR(ConfigRecord,*this);
}

std::wstring ConfigRecord::getValueAsString() const
{
	return m_valueString;
}
int ConfigRecord::getValueAsInt() const
{
	return m_valueInt;
}
double ConfigRecord::getValueAsDouble() const
{
	return m_valueDouble;
}

std::wstring ConfigRecord::getStringOfValue() const
{
	std::wostringstream os;
	std::wstring strForXmlOutput;
	switch(getType())
	{
	case ConfigRecordType::Int:
		os<<getValueAsInt();
		strForXmlOutput.assign(os.str());
		break;
	case ConfigRecordType::Double:
		os<<getValueAsDouble();
		strForXmlOutput.assign(os.str());
		break;
	case ConfigRecordType::String:
		strForXmlOutput.assign(getValueAsString());  //¸³Öµ²Ù×÷
		break;
	default:
		return L"";
	}
	return strForXmlOutput;
}

bool ConfigRecord::IsInternalIdentifyCodeValue() const
{
	return internalIdentifyCode==m_valueInt;
}