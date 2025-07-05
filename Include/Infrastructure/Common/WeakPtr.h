#pragma once

#include "WeakReferenceable.h"


namespace JackC
{
    namespace detail
    {
        template <class T,class U,class V=void>
        struct IsSafelyCastable:std::false_type{};

        template<typename T, typename U>
        struct IsSafelyCastable<T, U,std::void_t<decltype(static_cast<U>(std::declval<T>()) )>>
            :std::true_type
        {};
    }


    template <class T>
    class WeakPtr
    {
    public:
        /// <summary>
        /// 构造新的WeakPtr
        /// </summary>
        WeakPtr():m_pProxy(nullptr){}
        /// \brief 构造新的WeakPtr
         /// \param pObject 被引用对象的指针
        explicit WeakPtr(const T* pObject) : m_pProxy(nullptr)
        {
            Reset(pObject);
        }

        /// \brief 拷贝构造新的WeakPtr
        /// \param other 另一个WeakPtr
        WeakPtr(const WeakPtr& other) : m_pProxy(nullptr)
        {
            Reset(other.Get());
        }

        /// \brief 析构函数
        ~WeakPtr()
        {
            Reset(nullptr);
        }

    public:
        /// \brief 复制构造新的WeakPtr
           /// \param other 另一个WeakPtr
        WeakPtr& operator=(const WeakPtr& other)
        {
            Reset(other.Get());
            return *this;
        }

        /// \brief 复制构造新的WeakPtr
        /// \param pObject 被引用的对象
        WeakPtr& operator=(const T* pObject)
        {
            Reset(pObject);
            return *this;
        }

        /// \brief 判断是否和另一个WeakPtr相等
        /// \param other 另一个WeakPtr
        /// \return bool 如果和另一个WeakPtr引用同一个对象，返回true；否则返回false
        bool operator==(const WeakPtr& other)
        {
            return Get() == other.Get();
        }

        /// \brief WeakPtr是否是合法
        /// \return bool 合法返回true；不合法返回false
        bool IsValid() const
        {
            return m_pProxy && m_pProxy->IsValid();
        }

        /// \brief 重载operator bool操作符
        operator bool() const
        {
            return IsValid();
        }

        /// \brief 重载!操作符
        bool operator!() const
        {
            return !IsValid();
        }

        /// \brief 重载指针访问操作符
        T* operator->()
        {
            return Get();
        }

        /// \brief 重载指针访问操作符
        const T* operator->() const
        {
            return Get();
        }

        /// \brief 重载接引用操作符
        T& operator*()
        {
            return *Get();
        }

        /// \brief 重载接引用操作符
        const T& operator*() const
        {
            return *Get();
        }

        /// \brief 获取被引用对象的指针
        T* Get()
        {
            return Get(detail::IsSafelyCastable<WeakReferenceable*, T*>{});
        }

        /// \brief 获取被引用对象的指针
        const T* Get() const
        {
            return Get(detail::IsSafelyCastable<WeakReferenceable*, T*>{});
        }

        /// \brief 替换被引用的对象
        /// \param pObject 新的被引用的对象
        void Reset(const T* pObject = nullptr)
        {
            static_assert(std::is_base_of<WeakReferenceable, T>::value, "Object referenced by WeakPtr MUST be DERIVED from WeakReferenceable.");

            const T* pOldObject = Get();

            // 是否已经引用对象
            if (m_pProxy && pOldObject && pOldObject == pObject)
                return;

            // 解开老引用
            if (m_pProxy)
                m_pProxy->UnRef();

            // 引用到新的对象
            m_pProxy = pObject ? pObject->GetOrCreateProxy() : nullptr;
            if (m_pProxy)
                m_pProxy->Ref();
        }

    private:
        T* Get(std::true_type) {
            return static_cast<T*>(IsValid() ? m_pProxy->Get() : nullptr);
        }

        const T* Get(std::true_type) const {
            return static_cast<const T*>(IsValid() ? m_pProxy->Get() : nullptr);
        }

        T* Get(std::false_type) {
            return dynamic_cast<T*>(IsValid() ? m_pProxy->Get() : nullptr);
        }

        const T* Get(std::false_type) const {
            return dynamic_cast<const T*>(IsValid() ? m_pProxy->Get() : nullptr);
        }
        WeakReferenceable::Proxy* m_pProxy;
    };

}
