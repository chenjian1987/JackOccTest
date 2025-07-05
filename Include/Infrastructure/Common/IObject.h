#pragma once

#include "JDevService.h"
#include "WeakReferenceable.h"
#include "JQuickCast.h"
#include "JCommonDefine.h"

namespace JackC
{
    //所有接口的基类
    //提供弱引用，派生类可以用于WeakPtr<T>
    class JACKC_COMMON_INTERFACE_EXPORT IObject : public WeakReferenceable
    {
        DEFINE_CAST_BASE(JackC::IObject);
    public:
        virtual ~IObject()
        {

        }

        IObject()
        {
            int i = 0;
        }
    };
}
