#pragma once

#include "JDevService.h"
#include <atomic>
#include "JCommonDefine.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT WeakReferenceable
    {
    public:
        WeakReferenceable();
        virtual ~WeakReferenceable(void);

        WeakReferenceable(const WeakReferenceable& other);             //拷贝构造
        WeakReferenceable& operator=(const WeakReferenceable& other);  //赋值操作

    private:
        template <class T> friend class WeakPtr;

        class Proxy
        {
            CANNOT_COPY_OR_ASSIGN(Proxy);
        public:
            explicit Proxy(WeakReferenceable *pObject):m_pObject(pObject),m_refCount(0){}
            virtual ~Proxy()
            {
                if (m_pObject)m_pObject->m_pProxy = nullptr;
            }

            WeakReferenceable* operator->()
            {
                return m_pObject;
            }
            const WeakReferenceable* operator->() const
            {
                return m_pObject;
            }
            WeakReferenceable& operator*()
            {
                return *m_pObject;
            }
            const WeakReferenceable& operator*() const
            {
                return *m_pObject;
            }
            WeakReferenceable* Get()
            {
                return m_pObject;
            }
            const WeakReferenceable* Get() const
            {
                return m_pObject;
            }

            void Reset() { m_pObject = nullptr; }
            void Ref() { m_refCount++; }
            void UnRef() { if (--m_refCount == 0) delete this; }
            bool IsValid() { return m_pObject != nullptr; }
        private:
            friend class WeakReferenceable;
            WeakReferenceable* m_pObject;    //指向被代理的对象
            std::atomic<int> m_refCount;     //有多少waekptr引用到m_pObject
        };

        Proxy* GetOrCreateProxy() const;
    private:
        mutable Proxy* m_pProxy;

    };
}