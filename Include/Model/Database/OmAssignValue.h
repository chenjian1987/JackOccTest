#pragma once 
#include "OwnerPtr.h"
#include <type_traits>
#include "DebugMessage.h"
#include "TransferOwnership.h"


namespace JackC
{
    class IElement;
    class IDocument;

    template<class T>
    void AssignValue_DONT_USE(T& var, const T& value)
    {
        var = value;
    }

    template<class T>
    void AssignValue_DONT_USE(T*& var, const T* value)
    {
        var = (T*)value;
    }

    template<class T>
    void AssignValue_DONT_USE(OwnerPtr<T>& var, const OwnerPtr<T>& value)
    {
        OwnerPtr<T>* popValue = const_cast<OwnerPtr<T>*>(&value);
        var = TransferOwnership(*popValue);
    }

   /* template<class T>
    void AssignValue_DONT_USE(NdbVector<T>& var, const std::vector<T>& value)
    {
        var = value;
    }

    template<class T>
    void AssignValue_DONT_USE(NdbVector<OwnerPtr<T>>& var, const std::vector<OwnerPtr<T>>& value)
    {
        DBG_WARN(L"包含OwnerPtr的vector不支持直接赋值");
    }

    template<class T>
    void AssignValue_DONT_USE(NdbSet<T>& var, const std::set<T>& value)
    {
        var = value;
    }

    template<class T>
    void AssignValue_DONT_USE(NdbSet<OwnerPtr<T>>& var, const std::set<OwnerPtr<T>>& value)
    {
        DBG_WARN(L"包含OwnerPtr的set不支持直接赋值");
    }

    template<class T, class Comparer>
    void AssignValue_DONT_USE(NdbSet<T, Comparer>& var, const std::set<T, Comparer>& value)
    {
        var = value;
    }

    template<class T, class Comparer>
    void AssignValue_DONT_USE(NdbSet<OwnerPtr<T>, Comparer>& var, const std::set<OwnerPtr<T>, Comparer>& value)
    {
        DBG_WARN(L"包含OwnerPtr的set不支持直接赋值");
    }

    template<class K, class T>
    void AssignValue_DONT_USE(NdbMap<K, OwnerPtr<T> >& var, const std::map<K, OwnerPtr<T> >& value)
    {
        DBG_WARN(L"包含值为OwnerPtr的map不支持直接赋值");
    }

    template<class K, class T, class KeyComparer>
    void AssignValue_DONT_USE(NdbMap<K, OwnerPtr<T>, KeyComparer>& var, const std::map<K, OwnerPtr<T>, KeyComparer>& value)
    {
        DBG_WARN(L"包含值为OwnerPtr的map不支持直接赋值");
    }

    template<class K, class T>
    void AssignValue_DONT_USE(NdbMap<K, T>& var, const std::map<K, T>& value)
    {
        var = value;
    }

    template<class K, class T, class KeyComparer>
    void AssignValue_DONT_USE(NdbMap<K, T, KeyComparer>& var, const std::map<K, T, KeyComparer>& value)
    {
        var = value;
    }

    template<class K, class T, class Hash, class EqualTo = std::equal_to<K>>
    void AssignValue_DONT_USE(NdbUnorderedMap<K, OwnerPtr<T>, Hash, EqualTo>& var, const std::unordered_map<K, OwnerPtr<T>, Hash, EqualTo>& value)
    {
        DBG_WARN(L"包含值为OwnerPtr的unordered_map不支持直接赋值");
    }

    template<class K, class T, class Hash, class EqualTo = std::equal_to<K>>
    void AssignValue_DONT_USE(NdbUnorderedMap<K, T, Hash, EqualTo>& var, const std::unordered_map<K, T, Hash, EqualTo>& value)
    {
        var = value;
    }*/
};