#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT  DebugMessageMgr
    {
    private:
        DebugMessageMgr(void);
    public:
        ~DebugMessageMgr(void);

        //single form page
    public:
        static const DebugMessageMgr* GetDebugMessageMgr();
        static DebugMessageMgr* GetDebugMessageMgrFW();
    public:
        /// \brief 显示调试警告
        ///
        /// \param fileName      文件名
        /// \param lineNumber    行号
        /// \param functionName  函数名
        /// \param text          警告文本
        /// \param name          姓名
        /// \param date          日期
        /// \return 
        void ShowDebugWarn(const std::string& fileName, int lineNumber, const std::string& functionName, const std::wstring& text, const std::wstring& name, const std::wstring& date) const;

        /// \brief 注册为listener后在显示DebugWarnDialog前后有机会运行
        ///
        /// \param listener  回调函数
        /// \return 
        void RegisterAsPreShowListener(std::function<void()> listener);

        /// \brief  注册为listener后在显示DebugWarnDialog前后有机会运行
        ///
        /// \param listener  回调函数
        /// \return 
        void RegisterAsPostShowListener(std::function<void()> listener);

        /// \brief 标记即将退出Application，无需再ShowDebugWarn
        ///
        /// \param bWillQuitApplication  是否退出应用程序
        /// \return 
        void SetWillQuitApplication(bool bWillQuitApplication);

        /// \brief 是否已经有Debug Warn正在显示
        ///
        /// \return true: 是； false: 否
        bool IsShowingDebugWarn() const;

    public:
        /// \brief 调试输出
        ///
        /// \return
        void Dump() const;
    };
}



//
#define DBG_WARN(text) JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__,__LINE__, __FUNCTION__,text,L"jack.chen", L"2024");


#define DBG_WARN_UNLESS(condition,text) \
    if(!condition)  \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__,__LINE__, __FUNCTION__,text, L"jack.chen", L"2024");\
    }

#define DBG_WARN_AND_RETURN_UNLESS(condition,returnValue,text) \
    if(!(condition))  \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__,__LINE__, __FUNCTION__,text, L"jack.chen", L"2024");\
        return (returnValue);  \
    }

#define DBG_WARN_AND_RETURN_VOID_UNLESS(condition,text)\
    if(!(condition))  \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__,__LINE__, __FUNCTION__,text, L"jack.chen", L"2024");\
        return ;  \
    }

#define DBG_WARN_AND_RETURN_NULLPTR_UNLESS(condition, text) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, L"jack.chen", L"2024");\
        return nullptr; \
    }

#define DBG_WARN_AND_RETURN_FALSE_UNLESS(condition, text) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, L"jack.chen", L"2024"); \
        return false; \
    }

#define DBG_WARN_AND_CONTINUE_UNLESS(condition, text) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, L"jack.chen", L"2024"); \
        continue; \
    }

#define DBG_WARN_AND_BREAK_UNLESS(condition, text) \
    if(!(condition)) \
    { \
        JackC::DebugMessageMgr::GetDebugMessageMgr()->ShowDebugWarn(__FILE__, __LINE__, __FUNCTION__, text, L"jack.chen",  L"2024"); \
        break; \
    }
#pragma warning(pop)