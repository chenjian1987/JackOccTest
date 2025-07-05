#pragma once

namespace JackC
{
    class OneTimeInitializer
    {
    public:
        template <class FuncInitializer>
        OneTimeInitializer(FuncInitializer func)
        {
            func();
        }

        virtual ~OneTimeInitializer(void){}
    };
}

//一次性的初始化操作 可以使用这个宏定义处理，原理是构造一个静态对象，利用其构造函数进行指定的初始化操作
#define ONE_TIME_INITIALIZER(initRoutine) \
    JackC::OneTimeInitializer theInitializer_##initRoutine(initRoutine);

//一次性的初始化操作可以使用这个宏定义扩展,第一个参数 静态对象名称，用于区分同一个文件中的不同OneTimeInitializer静态对象。 第二个参数表示定义初始化操作，支持普通函数，函数，Lambda等
#define ONE_TIME_INITIALIZER_EXT(initRoutine,Func) \
    JackC::OneTimeInitializer theInitializer_##initRoutine(Func);