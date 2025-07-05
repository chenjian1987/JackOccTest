#include "UniqueString.h"
#include <mutex>
#include <unordered_map>

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

namespace 
{
	static std::unordered_map<std::string,UniqueString> s_uniqueStrings;
	static std::mutex s_uniqueStringMutex;
}
UniqueString::UniqueString(const std::string& content):m_content(content)
{
}
//支持多线程，也要考虑线程安全
const UniqueString* UniqueString::find(const std::string& content,bool createIfNotFound)
{
	std::lock_guard<std::mutex> lock(s_uniqueStringMutex);
	auto res=s_uniqueStrings.find(content);
	if(res!=s_uniqueStrings.end())  return &(res->second);   //end()获取unordered_map最后一个元素的迭代器位置
	if(createIfNotFound)
		return &(s_uniqueStrings.emplace(content,UniqueString(content)).first->second); //当 emplace() 添加新键值对失败时，说明容器中本就包含一个键相等的键值对，此时返回的迭代器指向的就是容器中键相同的这个键值对，bool 值为 False。
}


namespace 
{
	static std::unordered_map<std::wstring,UniqueWString> s_uniqueWStrings;
	static std::mutex s_uniqueWStringMutex;
}
UniqueWString::UniqueWString(const std::wstring& content):m_content(content)
{
}
//支持多线程，也要考虑线程安全
const UniqueWString* UniqueWString::find(const std::wstring& content,bool createIfNotFound)
{
	std::lock_guard<std::mutex> lock(s_uniqueWStringMutex);
	auto res=s_uniqueWStrings.find(content);
	if(res!=s_uniqueWStrings.end())  return &(res->second);
	if(createIfNotFound)
		return &(s_uniqueWStrings.emplace(content,UniqueWString(content)).first->second); //当 emplace() 添加新键值对失败时，说明容器中本就包含一个键相等的键值对，此时返回的迭代器指向的就是容器中键相同的这个键值对，bool 值为 False。
}