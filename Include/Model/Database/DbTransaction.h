#pragma once
#include "ElementDefine.h"
#include "DbUndoRedoBase.h"
#include "JDevService.h"


namespace JackC
{
    JACKC_ELEMENT_EXPORT class DbTransaction:public DbUndoRedoBase
    {
        CANNOT_COPY_OR_ASSIGN(DbTransaction);
    public:
        DbTransaction(Database* pDatabase, const std::wstring& name, int32_t id);
        DbTransaction(Database* pDatabase);


    public:
        virtual void PostAddElement(const ElementId& elementId) override;

    };
}