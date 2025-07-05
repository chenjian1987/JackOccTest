#pragma once
#include "JDevService.h"
#include <cstdint>
#include "JCommonDefine.h"
#include <JTypeDefs.h>


namespace JackC
{
    //用来表示软件运行期的唯一标识
    class JACKC_COMMON_INTERFACE_EXPORT RuntimeId
    {
    public:
        DEFINE_VISUAL_TYPE_INT64(JackC::IntBaseType::Dec)

    public:
         explicit RuntimeId(Int32 id = -1) : m_id(id) {}

         bool IsValid() const { return m_id >= 0; }
         Int32 AsInt32() const { return m_id; }
         bool operator == (const RuntimeId& another) const { return m_id == another.m_id; }
         bool operator != (const RuntimeId& another) const { return m_id != another.m_id; }
         bool operator < (const RuntimeId& another) const { return m_id < another.m_id; }
         bool operator > (const RuntimeId& another) const { return m_id > another.m_id; }
         RuntimeId& operator ++ () { ++m_id; return *this; }
         RuntimeId operator ++ (int) { RuntimeId old(*this); m_id++; return old; }

    public:
        int GetId()const {return int(m_id);}
 
    public:
        static const RuntimeId InvalidID;
        void SetId(Int32 id ) {m_id = id;}

    private:
        Int32 m_id;
    };
}



