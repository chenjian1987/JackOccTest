#pragma once
#ifndef MD5_H
#define MD5_H

#include <string>
#include <fstream>

/*´úÂëÁ´½Ó£ºhttps://www.oschina.net/question/565065_93999 */

/* Type define */
typedef unsigned char Byte_MD5;
typedef unsigned int uint32;

/* MD5 declaration. */
class MD5
{
public:
    MD5();

public:
    void update(const Byte_MD5* input, size_t length);
    void update(std::ifstream& in);
    void update(const void* input, size_t length);
    void final();
    void transform(const Byte_MD5 block[64]);
    void encode(const uint32* input, Byte_MD5* output, size_t length);
    void decode(const Byte_MD5* input, uint32* output, size_t length);
    std::string bytesToHexString(const Byte_MD5* input, size_t length);
    void reset();
    const Byte_MD5* digest();
    /* class uncopyable */
    MD5(const MD5&);
    MD5& operator=(const MD5&);

private:
    uint32 _state[4]; /* state (ABCD) */
    uint32 _count[2]; /* number of bits, modulo 2^64 (low-order word first) */
    Byte_MD5 _buffer[64]; /* input buffer */
    Byte_MD5 _digest[16]; /* message digest */
    bool _finished;   /* calculate finished ? */

    static const Byte_MD5 PADDING[64]; /* padding for calculate */
    static const char HEX[16];
    enum { BUFFER_SIZE = 1024 };
};

#endif /*MD5_H*/


