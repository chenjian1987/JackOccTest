#pragma once

#include "JDevService.h"
#include "OwnerPtr.h"
#include "JNew.h"
#include "JCommonDefine.h"

typedef unsigned char Byte_MD5;

class MD5;

namespace JackC
{
    // 计算MD5值接口
    class JACKC_COMMON_INTERFACE_EXPORT JMD5
    {
    public:
        // 默认构造函数
        JMD5();

        //带参构造函数
        JMD5(const void* input, size_t length);

        //带参构造函数
        JMD5(const std::string& str);

        //带参构造函数
        JMD5(std::ifstream& in);

        virtual ~JMD5();

    public:
        //向MD5对象内添加内存块
        void Update(const void* input, size_t length);

        //向MD5对象内添加字符串
        void Update(const std::string& str);

        //向MD5对象内添加输入流
        void Update(std::ifstream& in);

        //计算MD5码，并返回指向它的指针
        const Byte_MD5* Digest();

        //计算MD5码，并返回其对应的字符串
        std::string ToString();

        //重置
        void Reset();

    public:
        //计算文件的MD5值
        static std::wstring ComputeFileMD5Value(const std::wstring& filePath);

    private:
        OwnerPtr<MD5> m_oMd5;
    };
}
