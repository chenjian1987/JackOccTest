#pragma once
#include "JDevService.h"
#include "UniqueString.h"



//快速强制转换
#define DEFINE_CLASS_TYPE(classFullName)  \
		public: \
			static const char* ClassType() \
			{ \
			   static const char* s_classType=UniqueString::find(#classFullName,true)->getContent().c_str();  \
			   return s_classType; \
			} \
			virtual const char* ObjectKind() const \
			{\
				return ClassType();\
			}

#define DEFINE_QUERY_OBJECT_STATIC(classFullName) \
		template <class From> \
		static classFullName* QueryObjectStatic(const From* pFrom)\
		{\
			if(!pFrom) \
			   return nullptr; \
			return static_cast<classFullName*>(pFrom->QueryObject(classFullName::ClassType()));\
		}

#define DEFINE_CAST_BASE(classFullName) \
		DEFINE_CLASS_TYPE(classFullName)\
		virtual void* QueryObject(const char* type) const \
		{\
			if(!type)\
				return nullptr; \
			if(ClassType()==type) \
				return const_cast<classFullName*>(this); \
			return nullptr;\
		}\
		DEFINE_QUERY_OBJECT_STATIC(classFullName)

#define DEFINE_CAST_DERIVED_START(classFullName) \
		DEFINE_CLASS_TYPE(classFullName) \
		void* QueryObject(const char* type) const override \
		{\
			if(!type)\
				return nullptr;\
			if(type==ClassType()) \
				return const_cast<classFullName*>(this);

#define DEFINE_QUERY_BASE_OBJECT(baseClassFullName) \
		if(void* pRes=baseClassFullName::QueryObject(type)) \
			return pRes;
#define DEFINE_CAST_DERIVED_END()\
			return nullptr;\
	}\

	//在有继承体系的派生类中使用
#define DEFINE_CAST_DERIVED(classFullName,baseClassFullName) \
		DEFINE_CAST_DERIVED_START(classFullName) \
		DEFINE_QUERY_BASE_OBJECT(baseClassFullName)  \
		DEFINE_CAST_DERIVED_END() \
		DEFINE_QUERY_OBJECT_STATIC(classFullName)

	//在有继承体系的派生类中使用
#define DEFINE_CAST_DERIVED2(classFullName,baseClassFullName1,baseClassFullName2)\
		DEFINE_CAST_DERIVED_START(classFullName)\
		DEFINE_QUERY_BASE_OBJECT(baseClassFullName1)\
		DEFINE_QUERY_BASE_OBJECT(baseClassFullName2)\
		DEFINE_CAST_DERIVED_END()\
		DEFINE_QUERY_OBJECT_STATIC(classFullName)



namespace JackC
{
	#define quick_cast thread_safe_quick_cast

	//线程安全的类型快速转换
	template<class To,class From>
	const To* thread_safe_quick_cast(const From* pFrom)
	{
		static_assert(!std::is_pointer<To>::value,"quick_cast only accepts non-pointer type");
		return To::QueryObjectStatic(pFrom);
	}

	//线程安全的类型快速转换
	template<class To,class From>
	To* thread_safe_quick_cast(From* pFrom)
	{
		static_assert(!std::is_pointer<To>::value,"quick_cast only accepts non-pointer type");
		return To::QueryObjectStatic(pFrom);
	}
}
