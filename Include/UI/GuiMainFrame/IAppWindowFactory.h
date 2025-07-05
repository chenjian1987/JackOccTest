#pragma once

#include "GuiMainFrameDefine.h"
#include "OwnerPtr.h"


namespace JackC
{

    class IAppWindow;

    /// <summary>
    /// 获取程序窗口工厂类对象
    /// </summary>
    class JACKC_GUI_MAIN_FRAME_EXPORT IAppWindowFactory
    {
    public:
        /// <summary>
        /// 获取程序窗口工厂类对象
        /// </summary>
        /// <returns></returns>
        static IAppWindowFactory* Get();

        /// <summary>
        /// 程序窗口工厂类析构函数
        /// </summary>
        virtual ~IAppWindowFactory() {}

        /// <summary>
        /// 创建程序窗口方法
        /// </summary>
        /// <returns></returns>
        virtual OwnerPtr<IAppWindow> CreateAppWindow() = 0;

    protected:
        static IAppWindowFactory* ms_pFactory;
    };


}