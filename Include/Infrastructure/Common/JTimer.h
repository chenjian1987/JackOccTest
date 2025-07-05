#pragma once
#include "JDevService.h"
#include "JDevService.h"
#include "JCommonDefine.h"

#ifdef _WINDOWS
#include <Windows.h>
#else
//TODO-请添加相应系统版本实现
#endif

namespace JackC
{
    typedef void(*TimerCallback)(void* userData, unsigned long elapsed);

    /// <summary>
    /// 定时器类
    /// </summary>
    class JACKC_COMMON_INTERFACE_EXPORT JTimer
    {
    private:
        int64_t       m_timerId;
        int           m_interval;
        TimerCallback m_callback;
        void* m_userData;
    public:
        JTimer(int interval, TimerCallback callback, void* userData);
        virtual ~JTimer();

        /// <summary>
        /// 定时器是否有效
        /// </summary>
        /// <returns></returns>
        inline bool IsActive() const { return m_timerId >= 0; }

        /// <summary>
        /// 获取定时器ID
        /// </summary>
        /// <returns></returns>
        int64_t GetTimerId() const { return m_timerId; }

        /// <summary>
        /// 重置计时器时间
        /// </summary>
        /// <param name="msec"></param>
        void SetInterval(int msec);

        /// <summary>
        /// 设置计时器时间
        /// </summary>
        /// <returns></returns>
        int GetInterval() const;

        /// <summary>
        /// 启动定时器，启动成功后回调函数按照指定的时间间隔定期触发 
        /// </summary>
        void Start();

        /// <summary>
        /// 停止定时器 
        /// </summary>
        void Stop();

    private:
#ifdef _WINDOWS
        static void CALLBACK TimerProc(HWND, UINT, UINT_PTR idEvent, DWORD dwTime);
#else
        //请添加相应系统版本实现
#endif
    };
}
