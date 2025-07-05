#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"
#include "OwnerPtr.h"

namespace JackC
{
    //Debug Warn处理接口，如果没有通过Switch接口进行切换，默认为Assert操作
    class JACKC_COMMON_INTERFACE_EXPORT IDebugWarningDisplayer
    {
    public:
        //触发警告时的显示接口
        virtual bool Display(const std::string& fileName, int lineNum, const std::string& functionName, const std::wstring& text, const std::wstring& name, const std::wstring& data) = 0;

        //多线程下 debug warn是否弹出断言Assert
        virtual bool ShouldAssertInMultithread() const = 0;

        //获取当前的调试警告显示处理对象
        static IDebugWarningDisplayer* Get();

        //切换当前的调试警告处理对象
        static bool SwitchTo(OwnerPtr<IDebugWarningDisplayer> debugWarningDisplayer);

        //创建一个默认的Assert显示对象
        static OwnerPtr<IDebugWarningDisplayer> CreateAssertDisplayer();
    };
}
