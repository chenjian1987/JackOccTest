#pragma once
#include "DbObjectSchematic.h"
#include "ElementDefine.h"
#include "ElementId.h"


namespace JackC
{
    class JACKC_ELEMENT_EXPORT DbElement :public DbObjectSchematic
    {
        DEFINE_CAST_DERIVED(DbElement, DbObjectSchematic);
    public:
        static const ClassId& GetClassId()
        {
            static ClassId s_classId;
            return s_classId;
        }

    public:
        virtual ~DbElement(){}

        virtual DbObject* GetTopOwnerObject() const
        {
            return const_cast<DbElement*>(this);
        }

        virtual ElementId GetElementId() const = 0;
        virtual bool ValidatePropertyChange() const = 0;
        virtual void NotifyElementPreChange() = 0;
        virtual void NotifyElementPostChange() = 0;
    };
}