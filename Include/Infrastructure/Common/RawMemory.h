#pragma once

#include "JDefines.h"
#include "OwnerPtr.h"
#include "JCommonDefine.h"
#include "JDevService.h"

namespace JackC
{
    class JACKC_COMMON_INTERFACE_EXPORT RawMemory
    {
        CANNOT_COPY_OR_ASSIGN(RawMemory);   //不允许拷贝和赋值
    private:
        char* m_ptr;
        size_t m_size;
        bool m_constructed;
    private:
        //char* pBuffer = new char[sizeof(Student) + sizeof(int)];   1.提前分配内存
        //Student* pStudent = new(pBuffer) Student("jack", 36);      2.在已经分配内存的位置上建立对象
        //pStudent->~Student();                                      3.Student占用的空间释放
        //delete[] pBuffer;                                          4.彻底释放内存空间
        //在内存上构造对象，不做有效性检测
        template <class T>
        T* QuickConstructObject()
        {
            new(m_ptr)  T();
            m_constructed = true;
            return (T*)m_ptr;
        }
        //接受一个参数在内存块上构造对象，不做有效性检查
        template <class T, class Arg>
        T* QuickConstructObjec(Arg&& arg)
        {
            new(m_ptr) T(arg);
            m_constructed = true;
            return (T*)m_ptr;
        }
    public:
        RawMemory();
        //构造一块内存块，
        //@param zeroMemory：是否将内存块全部置0，构造对象时zeroMemory必须为true
        explicit RawMemory(size_t size, bool zeroMempry = false);
        virtual ~RawMemory();

    public:
        //获取内存块的首地址
        char* GetStartAddress() const { return m_ptr; }
        //内存块大小
        size_t GetSize() const { return m_size; }
        //重新分配内存块大小，
        void Resize(size_t size);
        // 是否是有效的内存块
        bool IsValid() const { return m_ptr && m_size > 0; }
        // 内存块是否是全0
        bool IsZeroMemory() const;
    public:
        //接受一个参数在内存上构造对象
        template <class T>
        static OwnerPtr<T> ConstructObjectOnZeroMemory()
        {
            RawMemory rawMemory((size_t)sizeof(T), true);
            rawMemory.QuickConstructObjec<T>();
            return rawMemory.ReleaseOwnership<T>();
        }
        // 在内存块上构造对象
        template<class T> T* ConstructObject()
        {
            //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T) == m_size, L"the class size must be equal to raw memory size.");
            //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(!m_constructed, L"object had been constructed before");
            //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsValid(), L"should not construct object on invalid raw memory");
            //DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsZeroMemory(), L"memory should all be zero.");

            return QuickConstructObject<T>();
        }
        // 接收一个参数的在内存块上构造对象
        template<class T, class Arg> T* ConstructObject(Arg&& arg)
        {
            /* DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T) == m_size, L"the class size must be equal to raw memory size.");
             DBG_WARN_AND_RETURN_NULLPTR_UNLESS(!m_constructed, L"object had been constructed before");
             DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsValid(), L"should not construct object on invalid raw memory");
             DBG_WARN_AND_RETURN_NULLPTR_UNLESS(IsZeroMemory(), L"memory should all be zero.");*/

            return QuickConstructObject<T>(std::forward<Arg>(arg));
        }
        // 将内存块所有权交给构造好的对象
        template<class T>
        OwnerPtr<T> ReleaseOwnership()
        {
            /*DBG_WARN_AND_RETURN_NULLPTR_UNLESS(sizeof(T) == m_size, L"the class size must be equal to raw memory size.");
            DBG_WARN_AND_RETURN_NULLPTR_UNLESS(m_constructed, L"object was not constructed");*/
            OwnerPtr<T> opT = OwnerPtr<T>((T*)m_ptr);
            m_ptr = 0;
            return opT;
        }
    };
}
