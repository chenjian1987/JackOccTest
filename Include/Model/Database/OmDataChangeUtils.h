#pragma once
#include "ElementDefine.h"
#include "DbObject.h"

namespace JackC
{
    class JACKC_ELEMENT_EXPORT OmDataChangeUtils
    {
    public:
        static bool ValidatePropertyChange(DbObject* pObject);
        static void NotifyElementPreChange(DbObject* pObject);
        static void NotifyElementPostChange(DbObject* pObject);
    };
}
