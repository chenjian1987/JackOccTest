#include <regex>
#include <sstream>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>

#include "JGuid.h"

using namespace JackC;


//匹配字符 '-'位置
int GuidMatchChar(const wchar_t* buffer, const wchar_t wchar)
{
    wchar_t* temp_buffer = (wchar_t*)buffer;
    int result = -1;
    while (*temp_buffer != '\0' && *temp_buffer != wchar)
    {
        temp_buffer++;
        result++;
    }
    if (*temp_buffer == '\0') return -1;  //not found
    if (result >= 0) result++;
    return result;
}

//获取指定字节转成无符号数
wchar_t* GuigGetNByteValue(const wchar_t* guidBuff, uint32_t& b32, int lenBuff = 0)
{
    wchar_t strImp[GUID_SIZE_ALLOC];
    if (lenBuff == 0) lenBuff = GuidMatchChar(guidBuff, '-');
    if (lenBuff >= 0)
    {
        wcsncpy(strImp, guidBuff, lenBuff); //wcsncpy将指定数量的宽字符从源复制到目标
        strImp[lenBuff] = '\0';
        b32 = (uint32_t)wcstoul(strImp, (wchar_t**)NULL, 16);   //将字符串转换成整数, 参数1-指向要转换的以\0结尾的宽字符串的指针,  参数2-指向宽字符的指针
    }
    if (lenBuff == 2)
    {
        return (wchar_t*)(guidBuff + lenBuff);
    }
    return (wchar_t*)(guidBuff + lenBuff+1);
}

//获取指定字节转成无符号数
wchar_t* GuigGetNByteValue(const wchar_t* guidBuff, uint16_t& b16, int lenBuff = 0)
{
    wchar_t strImp[GUID_SIZE_ALLOC];
    if (lenBuff == 0) lenBuff = GuidMatchChar(guidBuff, '-');
    if (lenBuff >= 0)
    {
        wcsncpy(strImp, guidBuff, lenBuff); //wcsncpy将指定数量的宽字符从源复制到目标
        strImp[lenBuff] = '\0';
        b16 = (uint16_t)wcstoul(strImp, (wchar_t**)NULL, 16);   //将字符串转换成整数, 参数1-指向要转换的以\0结尾的宽字符串的指针,  参数2-指向宽字符的指针
    }
    if (lenBuff == 2)
    {
        return (wchar_t*)(guidBuff + lenBuff);
    }
    return (wchar_t*)(guidBuff + lenBuff + 1);
}
//获取指定字节转成无符号数
wchar_t* GuigGetNByteValue(const wchar_t* guidBuff, uint8_t& b8, int lenBuff = 0)
{
    wchar_t strImp[GUID_SIZE_ALLOC];
    if (lenBuff == 0) lenBuff = GuidMatchChar(guidBuff, '-');
    if (lenBuff >= 0)
    {
        wcsncpy(strImp, guidBuff, lenBuff); //wcsncpy将指定数量的宽字符从源复制到目标
        strImp[lenBuff] = '\0';
        b8 = (uint8_t)wcstoul(strImp, (wchar_t**)NULL, 16);   //将字符串转换成整数, 参数1-指向要转换的以\0结尾的宽字符串的指针,  参数2-指向宽字符的指针
    }
    if (lenBuff == 2)
    {
        return (wchar_t*)(guidBuff + lenBuff);
    }
    return (wchar_t*)(guidBuff + lenBuff + 1);
}

unsigned long Random_Char()
{
    std::random_device rd;            //随机数发生器
    std::mt19937 gen(rd());           //随机数引擎
    std::uniform_int_distribution<> dis(0, 255);   //uniform_int_distribution特定范围内的非负数
    return static_cast<unsigned long> (dis(gen));
}

std::wstring CreateGuid()
{
    int len = 16;
    std::wstringstream woss;

    for (auto i = 0; i < len; i++)
    {
        auto rc = Random_Char();
        std::wstringstream hexStream;
        hexStream << std::hex << int(rc);
        auto hex = hexStream.str();
        woss << (hex.length() < 2 ? L'0' + hex : hex);

        if (i == 3 || i == 5 || i == 7 || i == 9)
        {
            woss << '-';
        }
    }
    return woss.str();
}


#pragma region constructors
JGuid::JGuid() :m_data1(0), m_data2(2), m_data3(0), m_data4{ 0 }{}
JGuid::JGuid(const std::wstring& guid) : m_data1(0), m_data2(2), m_data3(), m_data4{ 0 }
{
    wchar_t* tmpBuffer = (wchar_t*)guid.c_str();
    if (!IsGuidFormat(guid)) 
    {
    }
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data1);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data3);
    uint16_t m16b;
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m16b);
    m_data4[0] = (uint8_t)(m16b >> 8);
    m_data4[1] = (uint8_t)m16b;
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[2], 2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[3], 2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[4], 2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[5], 2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[6], 2);
    tmpBuffer = GuigGetNByteValue(tmpBuffer, m_data4[7], 2);

}

JGuid::JGuid(const uint32_t data1, const uint16_t data2, const uint16_t data3, const uint8_t  b1, const uint8_t  b2, const uint8_t  b3, const uint8_t  b4, const uint8_t  b5, const uint8_t  b6, const uint8_t  b7, const uint8_t  b8)
{
    m_data1 = data1;
    m_data2 = data2;
    m_data3 = data3;
    m_data4[0] = b1;
    m_data4[1] = b2;
    m_data4[2] = b3;
    m_data4[3] = b4;
    m_data4[4] = b5;
    m_data4[5] = b6;
    m_data4[6] = b7;
    m_data4[7] = b8;
}
JGuid::JGuid(uint32_t data1, uint16_t data2, uint16_t data3, uint8_t data4[8])
{
    m_data1 = data1;
    m_data2 = data2;
    m_data3 = data3;
    memcpy(m_data4, data4, sizeof(m_data4));
}
JGuid::JGuid(const JGuid& guid)
{
    m_data1 = guid.m_data1;
    m_data2 = guid.m_data2;
    m_data3 = guid.m_data3;
    memcpy(m_data4, guid.m_data4, sizeof(m_data4));
}
JGuid::JGuid(const Guid& guid)
{
    m_data1 = guid.Data1;
    m_data2 = guid.Data2;
    m_data3 = guid.Data3;
    memcpy(m_data4, guid.Data4, sizeof(m_data4));
}
#pragma endregion 

bool JGuid::operator<(const JGuid& guid) const
{
    if (memcmp(this, &guid, sizeof(JGuid)) < 0)
        return true;
    return false;
}
bool JGuid::operator>(const JGuid& guid) const
{
    if (memcmp(this, &guid, sizeof(JGuid)) > 0)
        return true;
    return false;
}
JGuid::operator Guid()
{
    return ToGuid();
}
Guid JGuid::ToGuid() const
{
    Guid result;

    result.Data1 = m_data1;
    result.Data2 = m_data2;
    result.Data3 = m_data3;
    memcpy(result.Data4, m_data4, sizeof(m_data4));
    return result;
}
Guid JGuid::Generate()
{
    return JGuid(CreateGuid());
}
JGuid JGuid::GetInvalid()
{
    static JGuid guid;
    return guid;
}

void JGuid::Assign(const JGuid& guid)
{
    m_data1 = guid.m_data1;
    m_data2 = guid.m_data2;
    m_data3 = guid.m_data3;
    memcpy(m_data4, guid.m_data4, sizeof(m_data4));
}
bool JGuid::IsSame(const JGuid& guid) const
{
    return !memcmp(this, &guid, sizeof(JGuid));
}
std::wstring JGuid::ToWString() const
{
    wchar_t tmpGuid[46] = { 0 };
    wmemset(tmpGuid, L'\0', 46);

    swprintf(tmpGuid,46, L"%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X",
        m_data1,
        m_data2,
        m_data3,
        m_data4[0],
        m_data4[1],
        m_data4[2],
        m_data4[3],
        m_data4[4],
        m_data4[5],
        m_data4[6],
        m_data4[7]);
    return tmpGuid;
}
std::wstring JGuid::ToWString(const Guid& guid)
{
    JGuid cGuid(guid);

    wchar_t tmpGuid[46] = { 0 };
    wmemset(tmpGuid, L'\0', 46);
    swprintf(tmpGuid,
        46,
        L"%.8x-%.4x-%.4x-%.2x%.2x-%.2x%.2x%.2x%.2x%.2x%.2x",
        cGuid.m_data1,
        cGuid.m_data2,
        cGuid.m_data3,
        cGuid.m_data4[0],
        cGuid.m_data4[1],
        cGuid.m_data4[2],
        cGuid.m_data4[3],
        cGuid.m_data4[4],
        cGuid.m_data4[5],
        cGuid.m_data4[6],
        cGuid.m_data4[7]);
    return std::wstring(tmpGuid);
}

bool JGuid::IsGuidFormat(const std::wstring& guid)
{
    const wchar_t *pRegex= LR"(^[0-9a-fA-F]{8}-([0-9a-fA-F]{4}-){2}([0-9a-fA-F]{2}){2}-([0-9a-fA-F]{2}){6}$)";
    const std::wregex hexPattern(pRegex);
    return std::regex_match(guid, hexPattern);
}
bool JGuid::Equal(const JGuid& guid1, const JGuid& guid2)
{
    return guid1 == guid2;
}
bool JGuid::Less(const JGuid& guid1, const JGuid& guid2)
{
    return guid1 < guid2;
}
bool JGuid::IsValid() const
{
    static JGuid guid;
    return guid != *this;
}
bool JGuid::IsValid(const JGuid& guid)
{
    static JGuid sGuid;
    return guid != sGuid;
}

int JGuid::Hash() const
{
    return (m_data1 ^ ((m_data2 << 0x10) | m_data3)) ^ ((m_data4[2] << 0x18) | m_data4[7]);

}