#pragma once
#include <unordered_map>

#include "ElementDefine.h"
#include "JDevService.h"
#include "UndoRedoCapability.h"
#include "ElementId.h"



namespace JackC
{
    class Database;
    class Element;

    class JACKC_ELEMENT_EXPORT DbUndoRedoBase
    {
    public:
        DbUndoRedoBase(Database* pDb, const std::wstring& name, int32_t id, UndoRedoCapability cap);
        DbUndoRedoBase(Database* pDb, UndoRedoCapability cap);
        virtual ~DbUndoRedoBase(){}

    public:
        UndoRedoCapability GetUndoRedoCapability() const;
        void SetUndoRedoCapability(UndoRedoCapability cap);
             
        virtual void PostAddElement(const ElementId& elementId);
        virtual void PreDeleteElement(const ElementId& elementId);
        virtual void PreModifyElement(const ElementId& elementId);

        virtual void RevertChanges();
        virtual bool HasChanges() const;
        virtual std::wstring GetName() const;

        const std::unordered_map<ElementId, const Element*, ElementIdHash>& GetAddElements() const;
        const std::unordered_map<ElementId, Element*, ElementIdHash>& GetDeleteElements() const;
        const std::unordered_map<ElementId, Element*, ElementIdHash>& GetModifyElements() const;

    public:
        Database* m_pDatabase;
        std::wstring m_name;
        int32_t m_id;
        UndoRedoCapability m_capability;

        std::unordered_map<ElementId, const Element*, ElementIdHash>        m_postAddElements;
        std::unordered_map <ElementId, Element*, ElementIdHash>   m_preModifyElements;    //修改过的元素
        std::unordered_map<ElementId, Element*, ElementIdHash>    m_preDeleteElements;    //准备删除的元素
    };
}