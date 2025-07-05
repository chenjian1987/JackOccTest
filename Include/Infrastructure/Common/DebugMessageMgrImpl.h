#pragma once
#include "JDevService.h"
#include "JCommonDefine.h"
#include "OwnerPtr.h"

#pragma warning(push)
#pragma warning(disable:4251)

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT DebugMessageMgrImpl
    {
    private:
        DebugMessageMgrImpl(void);

    public:
        virtual ~DebugMessageMgrImpl(void);

    public:
        static const DebugMessageMgrImpl* GetDebugMessageMgrImpl();
        static DebugMessageMgrImpl* GetDebugMessageMgrImplFW();

        // 弹出调试信息对话框，一般不直接使用，请使下面定义的宏，如DBG_WARN，DBG_WARN_UNLESS等。
    public:

        //显示调试警告
        void ShowDebugWarn(
            const std::string& fileName,
            int lineNumber,
            const std::string& functionName,
            const std::wstring& text,
            const std::wstring& name,
            const std::wstring& date) const;

        //注册为listener后在显示DebugWarnDialog前后有机会运行
        void RegisterAsPreShowListener(std::function<void()> listener);

        //注册为listener后在显示DebugWarnDialog前后有机会运行
        void RegisterAsPostShowListener(std::function<void()> listener);

        //标记即将退出Application，无需再ShowDebugWarn
        void SetWillQuitApplication(bool bWillQuitApplication);

        //是否已经有DebugWarning 正在显示
        bool IsShowingDebugWarn() const { return m_bIsShowingDebugWarn; }

        //判断当前线程在该线程所在的进程中是否为主线程
        bool IsMainThread() const;

        //判断是否应该显示DebugWarning的对话框，以下情况不显示对话框：
        bool ShouldShowDebugWarnDialog() const;

        // 调试输出
    public:
        void Dump() const;

    private:
        static JackC::OwnerPtr<DebugMessageMgrImpl> ms_upTheInstance;

        // 记录有哪些调试信息不再显示
        mutable std::set<std::wstring>     m_ignoredMessageLocations;
        std::vector<std::function<void()> > m_preShowListeners;
        std::vector<std::function<void()> > m_postShowListeners;
        bool m_bWillQuitApplication;
        mutable bool m_bIsShowingDebugWarn; // 是否已经有DebugWarning正在显示
    };

}
#pragma warning(pop)