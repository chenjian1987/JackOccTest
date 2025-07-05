#include "LogTest.h"

using namespace JackC;

void LogTest::LogManagerTest()
{
    std::wstring msg = L"hello,jack.chen!";

    LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::JOURNAL, msg, true);

}