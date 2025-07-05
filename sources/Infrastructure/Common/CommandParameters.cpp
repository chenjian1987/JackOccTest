#include "CommandParameters.h"
#include "StringUtils.h"
#include "JNew.h"
#include <regex>
#include "json/reader.h"
#include "json/writer.h"
#include "TransferOwnership.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "RuntimeId.h"



using namespace JackC;

namespace JackC
{
    class CommandParameterValueImpl
    {
    public:
        CommandParameterValueImpl() {}
        CommandParameterValueImpl(int32_t value) :m_val(value) {}
        CommandParameterValueImpl(uint32_t value) :m_val(value) {}
        CommandParameterValueImpl(int64_t value) :m_val(value) {};
        CommandParameterValueImpl(uint64_t value) :m_val(value) {};
        CommandParameterValueImpl(double value) :m_val(value) {};
        CommandParameterValueImpl(float value) :m_val(value) {};
        CommandParameterValueImpl(bool value) :m_val(value) {};
        CommandParameterValueImpl(const std::string& value) : m_val(value) {};
        CommandParameterValueImpl(const CommandParameterValueImpl& impl)
            :m_val(impl.getValue()) {}

        Json::Value& getValue() { return m_val; }
        const Json::Value& getValue() const { return m_val; }

    private:
        Json::Value m_val;
    };
}


CommandParameterValue::CommandParameterValue()
    :m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl))
{}

CommandParameterValue::CommandParameterValue(int32_t value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(uint32_t value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(int64_t value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(uint64_t value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(double value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(float value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(bool value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, value))
{}

CommandParameterValue::CommandParameterValue(const std::wstring& value)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, StringUtils::ToString(value)))
{}

CommandParameterValue::CommandParameterValue(const wchar_t* pValue): m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, StringUtils::ToString(pValue)))
{}

CommandParameterValue::CommandParameterValue(const CommandParameterValue& other)
    : m_impl(NEW_AS_OWNER_PTR(CommandParameterValueImpl, *(other.GetImpl().get())))
{}

CommandParameterValue::CommandParameterValue(CommandParameterValue&& value)
    : m_impl(TransferOwnership(value.GetImpl()))
{}

CommandParameterValue& CommandParameterValue::operator=(const CommandParameterValue& other)
{

    GetImpl()->getValue() = other.GetImpl()->getValue();
    return *this;
}

bool CommandParameterValue::operator<(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() < other.GetImpl()->getValue();
}
bool CommandParameterValue::operator<=(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() <= other.GetImpl()->getValue();
}

bool CommandParameterValue::operator>=(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() >= other.GetImpl()->getValue();
}
bool CommandParameterValue::operator>(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() > other.GetImpl()->getValue();
}
bool CommandParameterValue::operator==(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() == other.GetImpl()->getValue();
}
bool CommandParameterValue::operator!=(const CommandParameterValue& other) const
{
    //DBG_WARN_AND_RETURN_FALSE_UNLESS(other.GetImpl() != nullptr, L"CmdParameterImpl不存在");
    return GetImpl()->getValue() != other.GetImpl()->getValue();
}

int32_t CommandParameterValue::AsInt() const
{
    return GetImpl()->getValue().asInt();
}
uint32_t CommandParameterValue::AsUInt() const
{
    return GetImpl()->getValue().asUInt();
}
int64_t CommandParameterValue::AsInt64() const
{
    return GetImpl()->getValue().asInt64();
}
uint64_t CommandParameterValue::AsUInt64() const
{
    return GetImpl()->getValue().asUInt64();
}
float CommandParameterValue::AsFloat() const
{
    return GetImpl()->getValue().asFloat();
}
double CommandParameterValue::AsDouble() const
{
    return GetImpl()->getValue().asDouble();
}
bool CommandParameterValue::AsBool() const
{
    return GetImpl()->getValue().asBool();
}
std::wstring CommandParameterValue::AsWString() const
{
    return StringUtils::ToWString(GetImpl()->getValue().asString());
}


bool CommandParameterValue::IsNull() const
{
    return GetImpl()->getValue().isNull();
}
bool CommandParameterValue::IsBool() const
{
    return GetImpl()->getValue().isBool();
}
bool CommandParameterValue::IsInt() const
{
    return GetImpl()->getValue().isInt();
}
bool CommandParameterValue::IsInt64() const
{
    return GetImpl()->getValue().isInt64();
}
bool CommandParameterValue::IsUInt() const
{
    return GetImpl()->getValue().isUInt();
}
bool CommandParameterValue::IsUInt64() const
{
    return GetImpl()->getValue().isUInt64();
}
bool CommandParameterValue::IsDouble() const
{
    return GetImpl()->getValue().isDouble();
}
bool CommandParameterValue::IsWString() const
{
    return GetImpl()->getValue().isString();
}

std::wstring CommandParameterValue::Serialize(const CommandParameterValue& val)
{
    //DBG_WARN_AND_RETURN_UNLESS(val.GetImpl() != nullptr, L"", L"CmdParameterImpl不存在");
    Json::FastWriter jWriter;
    std::string dumpString = jWriter.write(val.GetImpl()->getValue());
    if (val.IsWString())
    {
        //这里主要用来处理序列化字符串时反斜杠的问题。
        std::string pattern = "\\\\";
        std::regex re(pattern);
        dumpString = std::regex_replace(dumpString, re, "\\\\");
        dumpString.insert(dumpString.find_last_of("\""), 1, '\\');
        return StringUtils::ToWString('\\' + dumpString);
    }

    return StringUtils::ToWString(dumpString);
}

CommandParameterValue CommandParameterValue::Unserialize(const std::wstring& str)
{
    Json::Reader jReader;
    Json::Value jValue;
    bool parseResult = jReader.parse(StringUtils::ToString(str), jValue, false);
    //DBG_WARN_AND_RETURN_UNLESS(parseResult, CommandParameterValue(), L"json parse error");

    OwnerPtr<CommandParameterValueImpl> impl = NEW_AS_OWNER_PTR(CommandParameterValueImpl);
    impl->getValue() = jValue;

    CommandParameterValue newValue;
    newValue.SetImpl(impl);

    return newValue;
}

void CommandParameterValue::SetImpl(OwnerPtr<CommandParameterValueImpl>& impl)
{
    m_impl = TransferOwnership(impl);
}

const OwnerPtr<CommandParameterValueImpl>& CommandParameterValue::GetImpl() const
{
    return m_impl;
}
OwnerPtr<CommandParameterValueImpl>& CommandParameterValue::GetImpl()
{
    return m_impl;
}

CommandParameterValue::~CommandParameterValue()
{}
