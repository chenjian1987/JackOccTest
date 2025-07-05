#include "StringTest.h"
#include "JGuid.h"
#include "OwnerPtr.h"
#include <JNew.h>
#include "FileTest.h"


#include "LogTest.h"
#include "WeakPtrTest.h"

#include "ModelViewTest.h"


using namespace JackC;


void main()
{
    //StringTest::Test1();

    //StringTest::filePathTest1();

    //StringTest::behaviorConfigTest();

    //JGuid guid(L"3E3916C8-9B89-4D05-893D-3A6C1A2418F7");
    //OwnerPtr<StringTest> ptStringTest = NEW_AS_OWNER_PTR(StringTest);;
    //ptStringTest.get()->guidTest2(guid);

    //StringTest::guidTest2(guid);


    //==============================================File Test=========================================================//
    //FileTest::LoadResourceFiles();


    //==============================================Log Test=========================================================//
    //OwnerPtr<LogTest> logTest;
    //logTest->LogManagerTest();


    //WeakPtrTest();


    //==============================================ModelView Test=========================================================//
    ModelViewTest modelViewTest;
    modelViewTest.TestCreateDefaultModelView();
}