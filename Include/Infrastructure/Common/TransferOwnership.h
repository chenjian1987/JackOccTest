#pragma once

#include <memory>
#include <utility>
#include "OwnerPtr.h"
#include "JQuickCast.h"

//转移资源所有权  模板函数
template <class T>
inline typename std::remove_reference<T>::type&& TransferOwnership(T&& unique_ptr)
{
    //std::remove_reference<T>::TYPE&& 确保无论T传入的是什么，都将返回一个真实类型的右值引用
    return std::move(unique_ptr);  //move将传入的参数以右值引用的方式返回
}
template<class T> inline std::shared_ptr<T> TransferOwnershipToSharedPtr(JackC::OwnerPtr<T>& opt)
{
    return std::shared_ptr<T>(opt.release());
}
template<class T> inline std::shared_ptr<T> TransferOwnershipToSharedPtr(JackC::OwnerPtr<T>&& opt)
{
    return std::shared_ptr<T>(opt.release());
}



/*!
*@brief  转移资源所有权(只有少量几个地方可以使用TransferOwnership_DontUse)
*@param[in] pointer 指向资源的指针
*/
template<class T> inline JackC::OwnerPtr<T> TransferOwnership_DontUse(T* pointer)
{
    return JackC::OwnerPtr<T>(pointer);
}




template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipCast(JackC::OwnerPtr<Base>& obj)
{
    Derived* pDerived = dynamic_cast<Derived*>(obj.get());
    if (pDerived != nullptr)
    {
        obj.release();
        return TransferOwnership_DontUse(pDerived);
    }
    return nullptr;
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipCast(JackC::OwnerPtr<Base>&& obj)
{
    Derived* pDerived = dynamic_cast<Derived*>(obj.get());
    if (pDerived != nullptr)
    {
        obj.release();
        return TransferOwnership_DontUse(pDerived);
    }
    return nullptr;
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipCast(Base* pObj)
{
    Derived* pDerived = dynamic_cast<Derived*>(pObj);
    if (pDerived != nullptr)
        return TransferOwnership_DontUse(pDerived);
    return nullptr;
}

// dynamic_cast很耗时，特定场景dynamic_cast不是必须的，可以使用static_cast。
// 从基类转换到派生类，慎用static_cast，只有在特定场景，确保不会发生意外才能使用static_cast。
template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(JackC::OwnerPtr<Base>& obj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(JackC::OwnerPtr<Base>&& obj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipStaticCast(Base* pObj)
{
    return TransferOwnership_DontUse(static_cast<Derived*>(pObj));
}


// quick_cast
template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(JackC::OwnerPtr<Base>& obj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(JackC::OwnerPtr<Base>&& obj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(obj.release()));
}

template<class Derived, class Base> inline
JackC::OwnerPtr<Derived> TransferOwnershipQuickCast(Base* pObj)
{
    return TransferOwnership_DontUse(JackC::quick_cast<Derived>(pObj));
}




#define TRANSFER_OWNERSHIP_TO_THIRD_PARTY(OWNER) (OWNER).release()