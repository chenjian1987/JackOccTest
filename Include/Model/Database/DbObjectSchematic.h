#pragma once
#include "DbObject.h"
#include "ElementDefine.h"
#include "ClassId.h"

namespace JackC
{
    class JACKC_ELEMENT_EXPORT DbObjectSchematic:public DbObject
    {
        DEFINE_CAST_DERIVED(DbObjectSchematic, DbObject);
    public:
        DbObjectSchematic(){}
        virtual ~DbObjectSchematic(){}

        virtual bool OnCloned(const DbObject* pOld) { return true; }
        virtual void OnUnserialized(){}
        static const ClassId& GetClassId()
        {
            static ClassId s_classId;
            return s_classId;
        }

    };
}
