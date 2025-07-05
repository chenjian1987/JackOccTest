#include "JDevService.h"
#include <cstdint>
#include "JCommonDefine.h"

namespace JackC
{
    //2017-11-8 10:30:45
    class JACKC_COMMON_INTERFACE_EXPORT  DateTime
    {
    private:
        DateTime();
    public:
        DateTime(int year, int month, int day);

        //单位秒
        DateTime(uint64_t time);

        ~DateTime();
    public:
        int GetDay() const;
        int GetMonth() const;
        int GetYear() const;
        int GetHour() const;
        int GetMinute() const;
        int GetSeconds() const;
        uint64_t GetTime() const;//1970年1月1日至今的秒数
        std::wstring ToString() const;
        //2017/11/8
        std::wstring ToDateString() const;
        bool operator ==(const DateTime& other) const { return m_time == other.m_time; }
        bool operator !=(const DateTime& other) const { return m_time != other.m_time; }
        bool operator <(const DateTime& other) const { return m_time < other.m_time; }
    public:
        //获取当前系统时间，精确到秒
        static DateTime Now();
        //获取当前系统时间，精确到微妙
        static uint64_t GetSystemTimeMicros();
    private:
        uint64_t m_time; //1970年1月1日至今的秒数
    };
}