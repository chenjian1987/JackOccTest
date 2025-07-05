#pragma once
#include "ElementDefine.h"
#include "WeakPtr.h"
#include "OwnerPtr.h"
#include "ElementId.h"
#include "EnumElementOperationType.h"
#include "DbTransaction.h"


namespace JackC
{
    class ElementManager;
    class ElementId;
    class Element;


    /// <summary>
    /// db，对外提供拷贝，事务，序列化功能
    /// </summary>
    class JACKC_ELEMENT_EXPORT Database
    {
    public:
        Database(ElementManager* pElementMgr);
        virtual ~Database(void);

    public:
        ElementManager* GetElementManagerFW();

        //Element添加之后 需要调用该函数来通知Database
        void OnPostAddDbElement(const ElementId& id);


    public:
        OwnerPtr<Element> CopyDbElementById(const ElementId& elementId);
        bool AddDbElementByIds(std::unordered_map<ElementId, Element*, ElementIdHash>& mapElements);
        bool DeleteDbElementByIds(const std::unordered_set<ElementId, ElementIdHash>& elementIds, EnumElementOperationType type);
        bool UpdateDbElementByIds(std::unordered_map<ElementId, Element*, ElementIdHash>& mapElements);


    private:
        WeakPtr<ElementManager>          m_wpElementMgr;
        OwnerPtr<DbTransaction>          m_opTransaction;

    };
}