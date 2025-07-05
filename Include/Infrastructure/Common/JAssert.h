#pragma once

#include <string>
#include "JCommonDefine.h"
#include "JDevService.h"
#include "AssertHelper.h"
#include "BasicConfigUtils.h"


#ifdef _WINDOWS
#include <crtdefs.h>

#undef Jack_assert_impl

#ifdef __cplusplus
extern "C" {
#endif
	_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t* _message,_In_z_ const wchar_t* _file,_In_ unsigned _line);

#ifdef __cplusplus
}
#endif
#define Jack_assert_impl(_Expression) (void)( (!!(_Expression)) || (_wassert(_CRT_WIDE(#_Expression), _CRT_WIDE(__FILE__), __LINE__), 0) )

#else 

#include <assert.h>
#define Jack_assert_impl(_Expression)  assert(_Expression)

#endif



namespace JackC
{
	bool JACKC_COMMON_INTERFACE_EXPORT IsAssertEnabled();
	void JACKC_COMMON_INTERFACE_EXPORT DumpAssertToLog(const std::string& expression, const std::string& file, int line, const std::string& function);
	void JACKC_COMMON_INTERFACE_EXPORT DumpDbgWarnToLog(const std::wstring& message, const std::wstring& file, int line);
}


//__FILE__:本行语句所在源文件的文件名
//__LINE__:本行语句在源文件的位置信息
//__FUNCTION__:所在的函数

#define JACK_ASSERT(EXPRESSION) \
    if(!(EXPRESSION)) \
    { \
        if( BasicConfigUtils::DebugMessageEnabled()) \
             AssertHelper::GetInstance()->Assert(_CRT_WIDE(#EXPRESSION), _CRT_WIDE(__FILE__), __LINE__);\
        else \
             DumpAssertToLog(#EXPRESSION, __FILE__, __LINE__, __FUNCTION__); \
    }

#define JACK_DBG_WARN(MESSAGE,FILENAME,LINE) \
	{\
		if(IsAssertEnabled())  \
			AssertHelper::GetInstance()->Assert(MESSAGE,FILENAME,LINE);  \
		else \
		    DumpDbgWarnToLog(MESSAGE,FILENAME,LINE); \
	}
