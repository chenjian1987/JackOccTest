#pragma once
#include "OwnerPtr.h"
#include "DebugMessage.h"

#define NEW_AS_OWNER_PTR(TYPE,...) JackC::OwnerPtr<TYPE>(new TYPE(__VA_ARGS__))

//[] 不捕获任何变量
//[&]捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）
//[=]捕获外部作用域中所有变量，并作为副本在函数体重使用（按值捕获）

#define NEW_AS_OCC(TYPE,...)\
    ([=]()->TYPE*{          \
        TYPE* re=new TYPE(__VA_ARGS__); \
        return re;                      \
    }())

#define NEW_AS_QT(T,...)   \
    ([=]()->T*{   \
        T* m_t=new T(__VA_ARGS__);  \
        return m_t;     \
    }())


//Qt only
#define NEW_AS_QT_CHILD(TYPE,...)\
    ([=]()->TYPE* { \
        static_assert(std::is_base_of<QObject,TYPE>::value,"only QT object is supported");  \
        TYPE* pQtChild=new TYPE(__VA_ARGS__);   \
        return pQtChild;    \
    }())


// 非顶层QLayout对象创建的时候没有parent，后面需要调用QLayout::addLayout()或者QWidget::setLayout()添加parent
#define NEW_AS_QT_LAYOUT(TYPE, ...) \
    ([=]()->TYPE*{ \
        static_assert(std::is_base_of<QLayout, TYPE>::value, "only QLayout object is supported."); \
        TYPE* pQLayout = new TYPE(__VA_ARGS__); \
        return pQLayout; \
    }())


//DBG_WARN_AND_RETURN_NULLPTR_UNLESS(pQtChild->parent(), L"parent cannot be null");     \