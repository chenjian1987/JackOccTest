#include <map>
#include "JTimer.h"
#include <Windows.h>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;

static std::map<UINT_PTR, JTimer*> TimerMap;  //UINT_PTR无符号的64位整型， 一般用来存储指针地址 unsigned int64  64bit

void CALLBACK JTimer::TimerProc(HWND, UINT, UINT_PTR idEvent, DWORD dwTime)   //DWORD: unsigned long 32bit    //UINT:unsigned int 32bit
{
    std::map<UINT_PTR, JTimer*>::iterator itor = TimerMap.find(idEvent);
    if (itor == TimerMap.end()) return;
    JTimer* it = itor->second;
    (*(it->m_callback))(it->m_userData, dwTime); //函数指针
}

JTimer::JTimer(int interval, TimerCallback callback, void* userData):m_interval(interval),m_timerId(-1),m_callback(callback),m_userData(userData)
{}

JTimer::~JTimer() { Stop(); }


void JTimer::SetInterval(int msec)
{
#ifdef _WINDOWS
    m_interval = msec;
    if (IsActive())
    {
        ::SetTimer(0, m_timerId, m_interval, &JTimer::TimerProc);
    }
#endif
}
int JTimer::GetInterval() const
{
    return m_interval;
}

//! @brief 启动定时器，启动成功后回调函数按照指定的时间间隔定期触发
void JTimer::Start()
{
#ifdef _WINDOWS
    m_timerId = ::SetTimer(0, 0, m_interval, &JTimer::TimerProc);
    if (m_timerId != 0)
    {
        TimerMap.insert(std::make_pair(m_timerId, this));
    }
#endif
}


void JTimer::Stop()
{
    if (m_timerId != -1)
    {
        if (::KillTimer(0, m_timerId))
        {
            std::map<UINT_PTR, JTimer*>::iterator itor = TimerMap.find(m_timerId);
            if (itor != TimerMap.end())
                TimerMap.erase(itor);
            m_timerId = -1;
        }
    }
}