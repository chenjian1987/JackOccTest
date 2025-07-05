#include "JMd5.h"
#include "DebugMessage.h"
#include "StringUtils.h"
#include "Md5.h"
#include <string>
#include <fstream>
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;
//

JMD5::JMD5()
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
}

JMD5::JMD5(const void* input, size_t length)
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(input, length);
}

JMD5::JMD5(const std::string& str)
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(str);
}

JMD5::JMD5(std::ifstream& in)
{
    m_oMd5 = NEW_AS_OWNER_PTR(MD5);
    Update(in);
}

JMD5::~JMD5()
{
    m_oMd5 = nullptr;
}

const Byte_MD5* JMD5::Digest()
{
    return m_oMd5->digest();
}

void JMD5::Reset()
{
    m_oMd5->reset();
}

void JMD5::Update(const void* input, size_t length)
{
    m_oMd5->update((const Byte_MD5*)input, length);
}

void JMD5::Update(const std::string& str)
{
    m_oMd5->update((const Byte_MD5*)str.c_str(), str.length());
}

void JMD5::Update(std::ifstream& in)
{
    m_oMd5->update(in);
}

std::string JMD5::ToString()
{
    return m_oMd5->bytesToHexString(Digest(), 16);
}

std::wstring JMD5::ComputeFileMD5Value(const std::wstring& filePath)
{
    std::ifstream in(filePath.c_str(), std::ios::binary);
    //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(in, L"inÎª¿Õ")

        JMD5 md5;
    std::streamsize length;
    const int oneSize = 1024;
    char buffer[oneSize];
    while (!in.eof())
    {
        in.read(buffer, oneSize);
        length = in.gcount();
        if (length > 0)
        {
            md5.Update(buffer, length);
        }
    }
    in.close();
    return StringUtils::ToWString(md5.ToString());
}
