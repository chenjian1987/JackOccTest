#pragma once
#include "ElementDefine.h"
#include "WeakReferenceable.h"
#include "JQuickCast.h"

namespace JackC
{
    class OmClassSchema;

    /// <summary>
    /// 数据对象基类,不需要直接继承，通过继承的DbObjectSchematic,DbObjectCloneable,DbElement等的派生类来使用
    /// </summary>
    class JACKC_ELEMENT_EXPORT Object:public WeakReferenceable
    {
        DEFINE_CAST_BASE(JackC::Object);
    public:
        explicit Object() {};
        virtual ~Object() {};
    };
}


