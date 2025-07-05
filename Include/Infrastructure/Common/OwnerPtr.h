#pragma once

#include <memory>
#include <type_traits>
#include <cassert>

namespace JackC
{
    template<class Pointer>
    struct ConstPointer
    {
        typedef typename std::remove_pointer<Pointer>::type RawPointer;
        typedef typename std::add_const<RawPointer>::type* Type;
    };

    //自定义删除器基类
    class DeleterBase
    {
    public:
        //解除对所指针的管理权
        virtual void release() = 0;

        //释放并销毁所管理的指针
        virtual void destory() = 0;

        // 释放自身
        virtual void delete_self() = 0;

        //创建一个空的删除器
        virtual DeleterBase* create() = 0;

        //判断当前删除器是否有效
        virtual bool is_valid() = 0;
    };

    //自定义删除器
    template <class T, class _Null = std::false_type>
    class OwnerPtrDeleter : public DeleterBase
    {
    public:
        OwnerPtrDeleter() : m_ptr(nullptr) {}
        OwnerPtrDeleter(std::nullptr_t ptr) : m_ptr(ptr) {}
        OwnerPtrDeleter(T* p) : m_ptr(p) 
        {
            auto name_p = typeid(p).name();
        }

        virtual DeleterBase* create() override { return new OwnerPtrDeleter<T>; }
        void set_resource(T* p) { m_ptr = p; }
        virtual void release() override { m_ptr = nullptr; }
        virtual void destory() override
        {
#pragma warning(push)
#pragma warning(disable: 4996)
            delete m_ptr;
#pragma warning(pop)
            m_ptr = nullptr;
        }
        virtual void delete_self() override
        {
            //判断当前删除器是否为空删除器, 否则需要释放Deleter。
            if (is_valid())
                delete this;
        }

        virtual bool is_valid() override { return !(_Null()); }

    private:
        T* m_ptr;
    };

    //自定义数组类型删除器
    template <class T, class _Null>
    class OwnerPtrDeleter<T[], _Null> : public DeleterBase
    {
    public:
        OwnerPtrDeleter() : m_ptr(nullptr) {}
        OwnerPtrDeleter(std::nullptr_t ptr) : m_ptr(ptr) {}
        OwnerPtrDeleter(T* p) : m_ptr(p) {}

        virtual DeleterBase* create() override { return new OwnerPtrDeleter<T[]>; }
        void set_resource(T* p) { m_ptr = p; }
        virtual void release() override { m_ptr = nullptr; }
        virtual void destory() override
        {
            delete[] m_ptr;
            m_ptr = nullptr;
        }
        virtual void delete_self() override
        {
            if (is_valid())
                delete this;
        }

        virtual bool is_valid() override { return !(_Null()); }
    private:
        T* m_ptr;
    };

    //获取默认的NullDeleter器
    static DeleterBase* __null_deleter()
    {
        static OwnerPtrDeleter<std::nullptr_t, std::true_type> nullDel;
        return &nullDel;
    }

    //是通过指针占有并管理另一对象，并在离开作用域时释放该对象的智能指针。
    ///
    /// 在下列两者之一发生时用关联的删除器释放对象：
    ///     1) 销毁了OwnerPtr对象
    ///     2) 通过 operator= 或 reset() 赋值另一指针给管理的OwnerPtr对象。
    /// 通过调用自定义删除器，保证即便在跨DLL的情况下，也能完成对资源的正确释放。
    /// OwnerPtr亦可以不占有对象，该情况下称它为空(empty)。
    /// 有两个版本：
    ///    1) 管理单个对象（例如以 new 分配）
    ///    2) 管理动态分配的对象数组（例如以 new[] 分配）
    /// 类满足可移动构造(MoveConstructible) 和可移动赋值(MoveAssignable) 的要求，
    /// 但不满足可复制构造(CopyConstructible) 或可复制赋值(CopyAssignable) 的要求。
    template <class T, class DT = OwnerPtrDeleter<T>, bool = std::is_same<DT, OwnerPtrDeleter<T>>::value>
    class OwnerPtr
    {
    public:
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type  Type;
        typedef typename std::remove_const<typename std::remove_reference<DT>::type>::type DeleterType;
        typedef Type* PointerType;
        typedef typename ConstPointer<PointerType>::Type ConstPointerType;

    public:
        //返回用于析构被管理对象的删除器
        DeleterBase* get_deleter()
        {
            return m_del;
        }

        //返回用于析构被管理对象的删除器
        const DeleterBase* get_deleter() const
        {
            return m_del;
        }

        //释放所拥有的删除器
        DeleterBase* release_deleter()
        {
            DeleterBase* old = m_del;
            m_del = __null_deleter();
            return old;
        }

        //构造新的空OwnerPtr对象
        OwnerPtr() : m_ptr(nullptr), m_del(__null_deleter()) 
        {
            auto name_ptr = typeid(m_ptr).name();
        }

        //构造新的空OwnerPtr对象
        OwnerPtr(std::nullptr_t ptr) :m_ptr(ptr), m_del(__null_deleter()) 
        {
            auto name_ptr = typeid(m_ptr).name();
        }

        //构造新的OwnerPtr对象
        explicit OwnerPtr(PointerType ptr)
            :m_ptr(ptr), m_del((ptr == nullptr) ? __null_deleter() : new DT(ptr)) 
        {
            auto name_ptr = typeid(m_ptr).name();
        }

        //构造新的OwnerPtr对象
        ///  该方法不会直接使用传过来的删除器，而是通过调用其create接口在Deleter
        ///  所在模块重新创建一个新的Deleter。
        OwnerPtr(PointerType ptr, const DeleterType& deleter) :m_ptr(ptr)
        {
            m_del = const_cast<DeleterType*>(&deleter)->create();
            ((DeleterType*)m_del)->set_resource(ptr);
        }

        //构造新的OwnerPtr对象
        OwnerPtr(PointerType ptr, DeleterType&& deleter) :m_ptr(ptr)
        {
            m_del = deleter.create();
            ((DeleterType*)m_del)->set_resource(ptr);
        }

        //移动构造新的OwnerPtr对象
        OwnerPtr(OwnerPtr&& ptr)
            :m_ptr(ptr.get()), m_del(ptr.release_deleter())
        {
            ptr.release();
        }
        //--
        OwnerPtr(OwnerPtr& ptr) :m_ptr(ptr.get()), m_del(ptr.release_deleter())
        {
            ptr.release();
        }

        //移动构造新的OwnerPtr对象
        template<class _Ty2, class _Dx2>
        OwnerPtr(OwnerPtr<_Ty2, _Dx2>&& other)
            : m_ptr(other.get()), m_del(other.release_deleter())
        {
            other.release();
        }

        //获取被管理的对象的指针
        PointerType get()
        {
            return m_ptr;
        }

        //获取被管理的对象的指针
        ConstPointerType get() const
        {
            return m_ptr;
        }

        //替换被管理对象
        void reset(PointerType ptr)
        {
            if (ptr != m_ptr)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_del = new DT(ptr);
                m_ptr = ptr;
            }
        }

        //替换被管理对象为空
        void reset(std::nullptr_t ptr = nullptr)
        {
            if (ptr != m_ptr)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_del = __null_deleter();
                m_ptr = nullptr;
            }
        }

        //返回一个指向被管理对象的指针，并释放所有权 
        PointerType release()
        {
            PointerType old = m_ptr;
            _get_deleter()->release();
            _get_deleter()->delete_self();
            m_del = __null_deleter();
            m_ptr = nullptr;
            return old;
        }

        //重载指针访问操作符
        PointerType operator->()
        {
            return m_ptr;
        }

        //重载指针访问操作符
        ConstPointerType operator->() const
        {
            return m_ptr;
        }

        //重载解引用操作符
        Type& operator*()
        {
            return *m_ptr;
        }

        //重载解引用操作符
        const Type& operator*() const
        {
            return *m_ptr;
        }

        //重载operator bool操作符
#ifdef __clang__
        explicit operator bool() const
#else
        operator bool() const
#endif
        {
            return static_cast<bool>(m_ptr != nullptr);
        }

        //重载移动赋值操作符
        OwnerPtr& operator=(std::nullptr_t ptr)
        {
            reset();
            m_ptr = nullptr;
            m_del = __null_deleter();
            return *this;
        }

        //重载移动赋值操作符
        OwnerPtr& operator=(OwnerPtr&& other)
        {
            if (this != &other)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_ptr = other.get();
                m_del = other.release_deleter();
                other.release();
            }
            return *this;
        }
        //jack
        OwnerPtr& operator=(const PointerType other)
        {
            _get_deleter()->destory();
            _get_deleter()->delete_self();
            m_ptr = other;
            m_del = __null_deleter();
            //other.release();
            return *this;
        }


        //--jack
        OwnerPtr& operator=(OwnerPtr& other)
        {
            if (this != &other)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_ptr = other.get();
                m_del = other.release_deleter();
                other.release();
            }
            return *this;
        }

        //重载移动赋值操作符
        template <class _Up, class _Ep>
        OwnerPtr& operator=(OwnerPtr<_Up, _Ep>&& other)
        {
            _get_deleter()->destory();
            m_ptr = other.get();
            _get_deleter()->delete_self();
            m_del = other.release_deleter();
            other.release();
            return *this;
        }
        virtual ~OwnerPtr()
        {
            if (m_ptr != nullptr)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_ptr = nullptr;
                m_del = nullptr;
            }
        }
    private:
        DeleterBase* _get_deleter()
        {
            //m_del 不应为空
            //当该断言被触发时， 需判断是否对全局或静态变量的初始化顺序有依赖
            assert(m_del != nullptr);
            return m_del;
        }
    private:
        OwnerPtr(const OwnerPtr&);

        template <class _Up, class _Ep>
        OwnerPtr(const OwnerPtr<_Up, _Ep>&);

        OwnerPtr& operator=(const OwnerPtr&);

        template <class _Up, class _Ep>
        OwnerPtr& operator=(const OwnerPtr<_Up, _Ep>&);

        //std::unique_ptr<T, DT> m_uptr;
        PointerType m_ptr;
        DeleterBase* m_del;
    };

    //管理动态分配的对象数组的OwnerPtr
    /// 详见管理单个对象的OwnerPtr
    template <class T, class DT>
    class OwnerPtr<T[], DT, true>
    {
    public:
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type  Type;
        typedef typename std::remove_const<typename std::remove_reference<DT>::type>::type DeleterType;
        typedef Type* PointerType;
        typedef typename ConstPointer<PointerType>::Type ConstPointerType;

    public:
        //返回用于析构被管理对象的删除器
        DeleterBase* get_deleter()
        {
            return m_del;
        }

        //返回用于析构被管理对象的删除器
        const DeleterBase* get_deleter() const
        {
            return m_del;
        }

        DeleterBase* release_deleter()
        {
            DeleterBase* old = m_del;
            m_del = __null_deleter();
            return old;
        }

        //构造新的空OwnerPtr对象
        OwnerPtr() : m_ptr(nullptr), m_del(__null_deleter()) {}

        //构造新的空OwnerPtr对象
        OwnerPtr(std::nullptr_t ptr) :m_ptr(ptr), m_del(__null_deleter()) {}

        //构造新的OwnerPtr对象
        explicit OwnerPtr(PointerType ptr)
            :m_ptr(ptr), m_del((ptr == nullptr) ? __null_deleter() : new DT(ptr)) {}

        OwnerPtr(PointerType ptr, const DeleterType& deleter) :m_ptr(ptr)
        {
            m_del = const_cast<DeleterType*>(&deleter)->create();
            ((DeleterType*)m_del)->set_resource(ptr);
        }

        //构造新的OwnerPtr对象
        OwnerPtr(PointerType ptr, DeleterType&& deleter) :m_ptr(ptr)
        {
            m_del = deleter.create();
            ((DeleterType*)m_del)->set_resource(ptr);
        }


        //移动构造新的OwnerPtr对象
        OwnerPtr(OwnerPtr&& ptr)
            :m_ptr(ptr.get()), m_del(ptr.release_deleter())
        {
            ptr.release();
        }

        //移动构造新的OwnerPtr对象
        template<class _Ty2, class _Dx2>
        OwnerPtr(OwnerPtr<_Ty2, _Dx2>&& other)
            : m_ptr(other.get()), m_del(other.release_deleter())
        {
            other.release();
        }

        //获取被管理的对象的指针
        PointerType get()
        {
            return m_ptr;
        }

        //获取被管理的对象的指针
        ConstPointerType get() const
        {
            return m_ptr;
        }

        //替换被管理对象
        /// \param ptr 用来替换当前被管理对象的新对象指针
        void reset(PointerType ptr)
        {
            if (ptr != m_ptr)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_del = new DT(ptr);
                m_ptr = ptr;
            }
        }

        //替换被管理对象为空
        void reset(std::nullptr_t ptr = nullptr)
        {
            if (ptr != m_ptr)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_del = __null_deleter();
                m_ptr = nullptr;
            }
        }

        //返回一个指向被管理对象的指针，并释放所有权 
        PointerType release()
        {
            PointerType old = m_ptr;
            _get_deleter()->release();
            _get_deleter()->delete_self();
            m_del = __null_deleter();
            m_ptr = nullptr;
            return old;
        }

        //重载指针访问操作符
        PointerType operator->()
        {
            return m_ptr;
        }

        //重载指针访问操作符
        ConstPointerType operator->() const
        {
            return m_ptr;
        }

        //获取某个被管理的对象的引用
        Type& operator[](size_t index)
        {
            return m_ptr[index];
        }

        //获取某个被管理的对象的引用
        const Type& operator[](size_t index) const
        {
            return m_ptr[index];
        }

        //重载operator bool操作符
#ifdef __clang__
        explicit operator bool() const
#else
        operator bool() const
#endif
        {
            return static_cast<bool>(m_ptr != nullptr);
        }

        //重载移动赋值操作符
        OwnerPtr& operator=(std::nullptr_t ptr)
        {
            reset();
            m_ptr = nullptr;
            m_del = __null_deleter();
            return *this;
        }

        //重载移动赋值操作符
        OwnerPtr& operator=(OwnerPtr&& other)
        {
            if (this != &other)
            {
                _get_deleter()->destory();
                _get_deleter()->delete_self();
                m_ptr = other.get();
                m_del = other.release_deleter();
                other.release();
            }
            return *this;
        }

        //重载移动赋值操作符
        template <class _Up, class _Ep>
        OwnerPtr& operator=(OwnerPtr<_Up, _Ep>&& other)
        {
            _get_deleter()->destory();
            m_ptr = other.get();
            _get_deleter()->delete_self();
            m_del = other.release_deleter();
            other.release();
            return *this;
        }
        virtual ~OwnerPtr()
        {
            _get_deleter()->destory();
            _get_deleter()->delete_self();
            m_ptr = nullptr;
            m_del = nullptr;
        }
    private:
        DeleterBase* _get_deleter()
        {
            //当这个assert被触发时， 需判断是否是对全局或静态变量的初始化顺序有不当依赖。
            assert(m_del != nullptr);
            return m_del;
        }
    private:
        PointerType m_ptr;
        DeleterBase* m_del;
    };

    //当需要自定义删除器时， 将使用std::unique_ptr替换
    /// 
    /// 需自行保证资源在跨DLL的情况下的安全。
    template<class _Ty, class _Dx>
    class OwnerPtr<_Ty, _Dx, false> : public std::unique_ptr<_Ty, _Dx>
    {
    public:
        typedef typename std::remove_const<typename std::remove_reference<_Ty>::type>::type  Type;
        typedef typename std::remove_const<typename std::remove_reference<_Dx>::type>::type DeleterType;
        typedef Type* PointerType;
        typedef typename ConstPointer<PointerType>::Type ConstPointerType;

    public:
        typedef typename std::unique_ptr<_Ty, _Dx> unique_ptr;
        typedef typename unique_ptr::pointer pointer;
        using unique_ptr::get_deleter;
        using unique_ptr::release;
        using unique_ptr::reset;

        constexpr OwnerPtr() noexcept
            : unique_ptr() {}

        constexpr OwnerPtr(std::nullptr_t _Ptr) noexcept
            : unique_ptr(_Ptr) {}

        explicit OwnerPtr(pointer _Ptr) noexcept
            : unique_ptr(_Ptr) {}

        OwnerPtr(pointer _Ptr, const _Dx& _Dt) noexcept
            : unique_ptr(_Ptr, std::move(_Dt)) {}

        OwnerPtr(pointer _Ptr, typename std::remove_reference<_Dx>::type&& _Dt) noexcept
            : unique_ptr(_Ptr, std::move(_Dt)) {}

        OwnerPtr& operator=(OwnerPtr&& _Right)
        {
            if (this != &_Right)
            {
                reset(_Right.release());
                get_deleter() = std::forward<_Dx>(_Right.get_deleter());
            }
            return *this;
        }

        template<class _Ty2, class _Dx2>
        OwnerPtr(OwnerPtr<_Ty2, _Dx2, false>&& _Right) noexcept
            : unique_ptr(_Right.release(), std::forward<_Dx2>(_Right.get_deleter())) {}

        template<class _Ty2, class _Dx2>
        OwnerPtr& operator=(OwnerPtr<_Ty2, _Dx2, false>&& _Right)
        {
            reset(_Right.release());
            get_deleter() = std::forward<_Dx2>(_Right.get_deleter());
            return *this;
        }

        template<class _Ty2, class _Dx2>
        OwnerPtr& operator=(OwnerPtr<_Ty2, _Dx2, true>&& _Right)
        {
            reset(_Right.release());
            get_deleter() = std::forward<_Dx2>(_Dx2());
            return *this;
        }
    };

    template<class _Ty, class _Dx>
    class OwnerPtr<_Ty[], _Dx, false>
        :public std::unique_ptr<_Ty[], _Dx>
    {
    public:
        typedef typename std::remove_const<typename std::remove_reference<_Ty>::type>::type  Type;
        typedef typename std::remove_const<typename std::remove_reference<_Dx>::type>::type DeleterType;
        typedef Type* PointerType;
        typedef typename ConstPointer<PointerType>::Type ConstPointerType;

    public:
        typedef typename std::unique_ptr<_Ty[], _Dx> unique_ptr;
        typedef typename unique_ptr::pointer pointer;
        using unique_ptr::get_deleter;
        using unique_ptr::release;
        using unique_ptr::reset;

        constexpr OwnerPtr() noexcept
            : unique_ptr() {}

        template<class _Uty>
        explicit OwnerPtr(_Uty _Ptr) noexcept
            : unique_ptr(_Ptr) {}

        template<class _Uty>
        OwnerPtr(_Uty _Ptr, const _Dx& _Dt) noexcept
            : unique_ptr(_Ptr, std::move(_Dt)) {}

        template<class _Uty>
        OwnerPtr(_Uty _Ptr, typename std::remove_reference<_Dx>::type&& _Dt) noexcept
            : unique_ptr(_Ptr, std::move(_Dt)) {}

        OwnerPtr(OwnerPtr<_Ty[], _Dx, false>&& _Right) noexcept
            : unique_ptr(_Right.release(), std::forward<_Dx>(_Right.get_deleter())) {}

        OwnerPtr& operator=(OwnerPtr&& _Right)
        {
            if (this != &_Right)
            {
                reset(_Right.release());
                get_deleter() = std::forward<_Dx>(_Right.get_deleter());
            }
            return *this;
        }

        template<class _Ty2, class _Dx2>
        OwnerPtr& operator=(OwnerPtr<_Ty2, _Dx2, false>&& _Right)
        {
            reset(_Right.release());
            get_deleter() = std::forward<_Dx2>(_Right.get_deleter());
            return *this;
        }

        template<class _Ty2, class _Dx2>
        OwnerPtr& operator=(OwnerPtr<_Ty2, _Dx2, true>&& _Right)
        {
            reset(_Right.release());
            get_deleter() = std::forward<_Dx2>(_Dx2());
            return *this;
        }

    };

    //operator == 通用模板
    template<typename _Tp, typename _Dp, typename _Up, typename _Ep>
    inline bool operator==(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return __x.get() == __y.get();
    }

    //operator != 通用模板
    template<typename _Tp, typename _Dp, typename _Up, typename _Ep>
    inline bool operator!=(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return __x.get() != __y.get();
    }

    //operator < 通用模板
    template<typename _Tp, typename _Dp, typename _Up, typename _Ep>
    inline bool operator<(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return __x.get() < __y.get();
    }

    //operator > 通用模板
    template<typename _Tp, typename _Dp,
        typename _Up, typename _Ep>
    inline bool operator>(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return (__y < __x);
    }

    //operator <= 通用模板
    template<typename _Tp, typename _Dp, typename _Up, typename _Ep>
    inline bool operator<=(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return !(__y < __x);
    }

    //>= 通用模板
    template<typename _Tp, typename _Dp,
        typename _Up, typename _Ep>
    inline bool operator>=(const JackC::OwnerPtr<_Tp, _Dp>& __x, const JackC::OwnerPtr<_Up, _Ep>& __y)
    {
        return !(__x < __y);
    }

    //第一个参数是nullptr_t的operator == 特化模板
    template <class _T1, class _D1>
    inline bool operator==(std::nullptr_t, const OwnerPtr<_T1, _D1>& right)
    {
        return !right;
    }

    //第二个参数是nullptr_t的operator == 特化模板
    template <class _T1, class _D1>
    inline bool operator==(const OwnerPtr<_T1, _D1>& left, std::nullptr_t)
    {
        return !left;
    }

    //第一个参数是nullptr_t的operator != 特化模板
    template <class _T1, class _D1>
    inline bool operator!=(std::nullptr_t, const OwnerPtr<_T1, _D1>& right)
    {
        return static_cast<bool>(right);
    }

    //第二个参数是nullptr_t的operator != 特化模板
    template <class _T1, class _D1>
    inline bool operator!=(const OwnerPtr<_T1, _D1>& left, std::nullptr_t)
    {
        return static_cast<bool>(left);
    }
}
