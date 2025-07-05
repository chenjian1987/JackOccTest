#pragma once

#include "JDevService.h"
#include "JCommonDefine.h"

#define GUID_SIZE 36
#define GUID_SIZE_ALLOC GUID_SIZE+1

typedef struct 
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} Guid ;

inline int operator==(const Guid& left, const Guid& right)
{
    //memcmp:参数1指向内存块的指针，参数2指向内存块的指针，参数3：要比较的字节数--比较两个内存块的值
    return !memcmp(&left, &right, sizeof(Guid));
}

namespace JackC
{
    struct JACKC_COMMON_INTERFACE_EXPORT GuidHasher
    {
        std::size_t operator()(const Guid& key) const
        {
            return (key.Data1 ^ ((key.Data2 << 0x10) | key.Data3)) ^ ((key.Data4[2] << 0x18) | key.Data4[7]);   //| 或操作    ^异或操作    <<左移操作符
        }
    };
    struct JACKC_COMMON_INTERFACE_EXPORT GuidEqual
    {
        bool operator()(const Guid& lhs, const Guid& rhs) const
        {
            return !memcmp(&lhs, &rhs, sizeof(Guid));
        }
    };
    struct JACKC_COMMON_INTERFACE_EXPORT GuidComparer
    {
        bool operator()(const Guid& lhs, const Guid& rhs) const
        {
            return memcmp(&lhs, &rhs, sizeof(Guid)) < 0;
        }
    };

    typedef std::unordered_set<Guid, GuidHasher, GuidEqual> GuidUnorderedSet;

    class JACKC_COMMON_INTERFACE_EXPORT JGuid
    {
    private:
        uint32_t m_data1;
        uint16_t m_data2;
        uint16_t m_data3;
        uint8_t m_data4[8];
    public:
        JGuid();
        //通过wstring创建guid， exp:12345678-1234-1234-1234-123456781234
        JGuid(const std::wstring& guid);
        JGuid(uint32_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8]);
        JGuid(const uint32_t data1, const uint16_t data2, const uint16_t data3, const uint8_t data41, const uint8_t data42, const uint8_t data43, const uint8_t data44, const uint8_t data45, const uint8_t data46, const uint8_t data47, const uint8_t data48);
        //拷贝构造
        JGuid(const JGuid& guid);
        //Guid 
        JGuid(const Guid& guid);

        //translate the guid to unicode string 
        std::wstring ToWString() const;
        static std::wstring ToWString(const Guid& guid);

        bool IsSame(const JGuid& guid) const;

        bool operator==(const JGuid& guid) const
        {
            return IsSame(guid);
        }
        bool operator<(const JGuid& guid) const;
        bool operator>(const JGuid& guid) const;
        bool operator!=(const JGuid& guid) const
        {
            return !IsSame(guid);
        }
        void operator=(const JGuid& guid)
        {
            Assign(guid);
        }
        operator Guid();

        void Assign(const JGuid& guid);

        Guid ToGuid() const;

        static bool IsGuidFormat(const std::wstring& guid);

        static JGuid GetInvalid();

        bool IsValid() const;

        static bool IsValid(const JGuid& guid);

        /// <summary>
        /// Returns True  when the two GUID are the same.
        /// </summary>
        /// <param name="cubeGuid1"></param>
        /// <param name="cubeGuid2"></param>
        /// <returns></returns>
        static bool Equal(const JGuid& cubeGuid1, const JGuid& cubeGuid2);

        static bool Less(const JGuid& cubeGuid1, const JGuid& cubeGuid2);

        static Guid Generate();

        //! Hash function for GUID.
        int Hash() const;

        //! Computes a hash code for the given GUID of the Standard_Integer type, in the range [1, theUpperBound]
        //! @param theGUID the GUID which hash code is to be computed
        //! @param theUpperBound the upper bound of the range a computing hash code must be within
        //! @return a computed hash code, in the range [1, theUpperBound]
        static int HashCode(const JGuid& theGUID, int theUpperBound);


    };

}




