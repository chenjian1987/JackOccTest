#include "WeakPtrTest.h"
#include "WeakPtr.h"
#include "JNew.h"
#include <iostream>
#include <list>
#include "IObject.h"
#include "IEvent.h"

using namespace JackC;
//



void WeakPtrTest()
{
    const std::wstring str_id = L"jack";
    OwnerPtr<IObject> opObj = NEW_AS_OWNER_PTR(IObject);


    std::list<WeakPtr<IObject>> lsObjs;
    //WaekPtr
    WeakPtr<IObject> wpObj(opObj.get());
    lsObjs.emplace_back(wpObj);



}