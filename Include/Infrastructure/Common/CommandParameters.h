#pragma once

#include "JDevService.h"
#include <unordered_map>
#include "OwnerPtr.h"
#include "JCommonDefine.h"

namespace JackC
{
    class CommandParameterValueImpl;

    //命令参数的Value类型定义
    class JACKC_COMMON_INTERFACE_EXPORT CommandParameterValue
    {
    public:
        CommandParameterValue();
        CommandParameterValue(int32_t value);
        CommandParameterValue(uint32_t value);
        CommandParameterValue(int64_t value);
        CommandParameterValue(uint64_t value);
        CommandParameterValue(double value);
        CommandParameterValue(float value);
        CommandParameterValue(bool value);
        CommandParameterValue(const std::wstring& value);
        CommandParameterValue(const wchar_t* pValue);

        CommandParameterValue(const CommandParameterValue& value);
        CommandParameterValue(CommandParameterValue&& value);
        CommandParameterValue& operator=(const CommandParameterValue& other);

        //重载 <运算符
        bool operator<(const CommandParameterValue& other) const;
        bool operator<=(const CommandParameterValue& other) const;
        bool operator>=(const CommandParameterValue& other) const;
        bool operator>(const CommandParameterValue& other) const;
        bool operator==(const CommandParameterValue& other) const;
        bool operator!=(const CommandParameterValue& other) const;

        //参数值转换
        int32_t AsInt() const;
        uint32_t AsUInt() const;
        int64_t AsInt64() const;
        uint64_t AsUInt64() const;
        float AsFloat() const;
        double AsDouble() const;
        bool AsBool() const;
        std::wstring AsWString() const;

        //判断是否为空
        bool IsNull() const;

        //判断是否为Bool类型
        bool IsBool() const;

        //判断是否为Int类型
        bool IsInt() const;

        //判断是否为Int64类型
        bool IsInt64() const;

        //判断是否为UInt类型
        bool IsUInt() const;

        //判断是否为UInt64类型
        bool IsUInt64() const;

        //判断是否为Double类型
        bool IsDouble() const;

        //判断是否为wstring类型
        bool IsWString() const;

        //将命令参数值序列化成wstring 类型字符串
        static std::wstring Serialize(const CommandParameterValue& val);

        //从字符串中反序列出命令参数值
        static CommandParameterValue Unserialize(const std::wstring& str);
        virtual ~CommandParameterValue();
    protected:
        void SetImpl(OwnerPtr<CommandParameterValueImpl>& impl);
        const OwnerPtr<CommandParameterValueImpl>& GetImpl() const;
        OwnerPtr<CommandParameterValueImpl>& GetImpl();
    private:
        OwnerPtr<CommandParameterValueImpl> m_impl;
    };

    //命令参数的Key类型定义
    using CommandParameterKey = std::wstring;

    //命令参数类型定义
    //由于命令参数操作大部分都是插入和查找操作， 所以当前使用std::unordered_map
    class JACKC_COMMON_INTERFACE_EXPORT CommandParameters : public std::unordered_map<CommandParameterKey, CommandParameterValue> {};

}


