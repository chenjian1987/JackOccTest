#pragma once
#include "JCommonDefine.h"
#include "JGuid.h"

using namespace JackC;

class StringTest
{
public:
    static void Test1();

    static void filePathTest1();

    static void behaviorConfigTest();

    static void guidTest();

    static void guidTest2(const JGuid guid);
};