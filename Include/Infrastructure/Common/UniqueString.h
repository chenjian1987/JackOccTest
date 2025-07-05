#pragma once
#include "JDevService.h"
#include "JCommonDefine.h"
#include <string>

namespace JackC
{
	//用于保存全局、静态字符串。 保证相同内容的全局、静态字符串内存唯一。 避让相同内容的字符串的多次构造、拷贝
	//  另外，字符串比较，比较性能差。 在保证相同内容的全局、静态字符串内存唯一的情况， 可以直接通过比较地址来判断字符串是否相同
	class UniqueString
	{
	private:
		UniqueString(const std::string& content);
		std::string m_content;
	public:
		//查找string
		//  一般用于全局或静态字符串
		// \param: content UTF8格式
		// \param:createIfNotFound:如果没有找到，是否创建，默认为创建
		JACKC_COMMON_INTERFACE_EXPORT static const UniqueString* find(const std::string& content, bool createIfNotFound = true);
		const std::string& getContent() const
		{
			return m_content;
		}
	};


	//用于保存全局、静态宽字符串。 保证相同内容的全局、静态字符串内存唯一。 避让相同内容的字符串的多次构造、拷贝
	//  另外，字符串比较，比较性能差。 在保证相同内容的全局、静态字符串内存唯一的情况， 可以直接通过比较地址来判断字符串是否相同
	class UniqueWString
	{
	private:
		UniqueWString(const std::wstring& content);
		std::wstring m_content;
	public:
		//查找string
		//  一般用于全局或静态字符串
		// \param: content UTF8格式
		// \param:createIfNotFound:如果没有找到，是否创建，默认为创建
		JACKC_COMMON_INTERFACE_EXPORT static const UniqueWString* find(const std::wstring& content, bool createIfNotFound = true);
		const std::wstring& getContent() const
		{
			return m_content;
		}
	};
}
