#pragma once

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#define DLL_IMPORT
#endif

// 直接遍历容器元素
#if defined __cplusplus
#define FOR_EACH(ITEM, VECTOR) \
    for(auto& ITEM : VECTOR)
#else
#error FOR_EACH not implemented
#endif


// 直接遍历容器元素
#ifdef __clang__
void StaticAssertFalse(const char* file, int line, const char* comments);
#define STATIC_ASSERT_FALSE(COMMENTS) \
    StaticAssertFalse(__FILE__, __LINE__, COMMENTS)
#elif defined _MSC_VER
#define STATIC_ASSERT_FALSE(COMMENTS) \
    static_assert(false, COMMENTS);
#else
#error STATIC_ASSERT_FALSE not implemented
#endif

//直接比那里容器元素 emscriptem
#ifdef EMSCRIPTEM
#define FORCE_INLINE inline
#elif defined _MSC_VER
#define FORCE_INLINE __forceinline
#elif defined __GNUC__
#define FORCE_INLINE __attribute__ ((__always_inline__))
#else 
#error FORCE_INLINE not implemented
#endif


#define  FINAL_INTERFACE //只提供内部实现的接口，不希望外部继承。

//用于表明一个类是不允许被拷贝或赋值的
#define CANNOT_COPY_OR_ASSIGN(ClassName) \
    private: \
        ClassName(const ClassName&); \
        ClassName& operator=(const ClassName &);

//表明一个类不允许被赋值
#define CANNOT_ASSIGN(CLASSNAME)    \
    private:    \
        CLASSNAME& operator=(const CLASSNAME&);


#ifdef _MSC_VER
#define WSTRING_OF(NAME) L#NAME
#elif defined __clang__
#define WSTRING_OF(NAME) L""#NAME
#else
#error WSTRING_OF not implemented
#endif


// 按索引 [0..size-1] 遍历
#define FOR_EACH_INDEX(INDEX, VECTOR) \
    for (int INDEX = 0; INDEX < (int)VECTOR.size(); ++INDEX)


// 指定父类名称为Super
#define DECLARE_SUPER(PARENT) private: typedef PARENT Super;