#pragma once
#include "OmPropertySchemaType.h"
#include "OmDataChangeUtils.h"
#include "JTypeDefs.h"
#include "JAssert.h"
#include "OmAssignValue.h"
#include "OmClassSchema.h"
#include "OmClassSchemaManager.h"






//serializable 可序列化的
#define SERIALIZABLE_ENUM_CLASS(ENUM_TYPE)  \
        struct _NdbEnumClass##ENUM_TYPE: std::true_type{};   \
        enum class ENUM_TYPE:int32_t

#define DONT_CHECK_REGEN
#define COMMA ,

#define OFFSET(structure,member) offsetof(structure,member)

#define TYPE_NAME(TYPE,NAME) TypeDefTrait<TYPE>::RealName(NAME)

//命名空间的限定
#ifdef _MSC_VER
#define NDB_CHECK_FULL_NAMESPACE(CLASSNAME)\
        {   \
            if( (std::string("class ")+std::string(#CLASSNAME))!=typeid(CLASSNAME).name())   \
            {  JACK_ASSERT(!"please use full namespace"); }   \
        }
#endif

//class继承关系限定
#define NDB_CHECK_RELATIONSHIP(CHILD, PARENT) \
        {\
            if (!std::is_base_of<PARENT, CHILD>::value)\
            {\
                JACK_ASSERT(!#PARENT" is not base of "#CHILD);\
            }\
        }




    //属性附属器  return_type, name ,getter, setter,regen_checker再生check, behaviour_types行为类型
#define MEMBER_DATA_ACCESSOR(RETURN_TYPE,NAME,GETTER,SETTER,REGEN_CHECKER,BEHAVIOUR_TYPES)\
        protected:RETURN_TYPE const& GETTER##__() const \
        {   \
            REGEN_CHECKER;      \
            return m_##NAME;    \
        }   \
        protected: RETURN_TYPE& GETTER##FW__()  \
        {   \
            if((BEHAVIOUR_TYPES) & (Int32)EnumPropertyBehaviourType::SupportTransaction)    \
            {   \
                REGEN_CHECKER; \
                OmDataChangeUtils::ValidatePropertyChange(this);    \
                OmDataChangeUtils::NotifyElementPreChange(this);    \
            }   \
            return m_##NAME;    \
        }   \
        protected:  void SETTER##__(const RETURN_TYPE &value)   \
        {   \
            if((BEHAVIOUR_TYPES) & (Int32)EnumPropertyBehaviourType::SupportTransaction)   \
            {   \
                OmDataChangeUtils::ValidatePropertyChange(this);    \
                OmDataChangeUtils::NotifyElementPreChange(this);    \
            }   \
            AssignValue_DONT_USE(m_##NAME,value);\
            if((BEHAVIOUR_TYPES) & (Int32)EnumPropertyBehaviourType::SupportTransaction)   \
            {   \
                OmDataChangeUtils::NotifyElementPostChange(this);    \
            }   \
        }

#define NDB_CONTAINER_RAW_DATA_GETTER(RETURN_TYPE,NAME,GETTER)  \
        protected: const RETURN_TYPE& GETTER##__() const    \
        {   \
            return m_##NAME;    \
        }   \

#define CHECK_INT_TYPE(TYPE) \
        const wchar_t* realName = TYPE_NAME(TYPE, WSTRING_OF(TYPE)); \
        const wchar_t* typeName = WSTRING_OF(TYPE);\




    //#define MEMBER_DATA_CUSTOM(TYPE,NAME,BEHAVIOUR_TYPES)   \
    //        AddNdb##NAME##Property();   \
    //    }   \
    //    private:    \
    //        TYPE m_##NAME;  \
    //        MEMBER_DATA_ACCESSOR(TYPE,NAME,Get##NAME,Set##NAME,DONT_CHECK_REGEN,BEHAVIOUR_TYPES);   \
    //    private:    \
    //        static void AddNdb##NAME##Property()    \
    //        {   \
    //            if(IsIntegral<TYPE>::value) {CHECK_INT_TYPE(TYPE);} \
    //            if(std::is_same<TYPE,bool>::value){ JACK_ASSERT(!"please use bool type");}    \
    //            unsigned offset=OFFSET(Self,m_##NAME);  \
    //            static_assert(!(std::is_pointer<TYPE>::value && !std::is_base_of<DbObjectSchematic,Self>::value),"cannot have raw pointer" );   \
    //            if(OmPropertySchemaType::NativePointer==InferTypeAndAddPropertySchema( (TYPE*)0,ms_pNdbClassSchema,WSTRING_OF(NAME),TYPE_NAME(TYPE,WSTRING_OF(TYPE) ) ,offset,BEHAVIOUR_TYPES)  )   \
    //            {   \
    //             if( (BEHAVIOUR_TYPES) & (Int32)EnumPropertyBehaviourType::SupportArch) && !std::is_same<IElement*,TYPE>>::value&&!std::is_same<IDocument*,TYPE>::value) \
    //             {   \
    //                JACK_ASSERT(!"Raw pointer as Property");    \
    //             }   \
    //          }

    //AddNdb##NAME##Property();   
#define MEMBER_DATA_CUSTOM(TYPE,NAME,BEHAVIOUR_TYPES) \
        }   \
        private:    \
            TYPE m_##NAME;  \
            MEMBER_DATA_ACCESSOR(TYPE,NAME,Get##NAME,Set##NAME,DONT_CHECK_REGEN,BEHAVIOUR_TYPES);   \
        private:    \
            static void AddNdb##NAME##Property()    \
            {   \

#define MEMBER_ARR_DATA_CUSTOM(TYPE,NAME,BEHAVIOUR_TYPES) \
            AddNdb##NAME##Property();   \
        }   \
        private:    \
            std::vector<TYPE> m_##NAME;  \
            MEMBER_DATA_ACCESSOR(std::vector<TYPE>,NAME,Get##NAME,Set##NAME,DONT_CHECK_REGEN,BEHAVIOUR_TYPES);   \
            typedef std::vector<TYPE> Vector##NAME; \
        protected:  \
            const Vector##NAME& Get##NAME##AsVector__()     \
            {   \
                return m_##NAME;        \
            }   \
        private:    \
            static void AddNdb##NAME##Property()    \
            {   \



    //属性赋予宏
#define MEMBER_DATA(TYPE,NAME)  \
        MEMBER_DATA_CUSTOM(TYPE,NAME,(Int32)EnumPropertyBehaviourType::SupportAll)

#define MEMBER_DATA_TRANSIENT(TYPE,NAME) \
        MEMBER_DATA_CUSTOM(TYPE,NAME,(Int32)EnumPropertyBehaviourType::Transing)

#define MEMBER_ARR_DATA(TYPE,NAME)  \
        MEMBER_ARR_DATA_CUSTOM(TYPE,NAME,(Int32)EnumPropertyBehaviourType::SupportAll)


    //#define MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)    \
    //    public: \
    //        static const ClassId& GetClassId()  \
    //        {   \
    //            static ClassId s_classId(WSTRING_OF(y_classId)); return s_classId;  \
    //        }   \
    //        typedef y_parentName Super; \
    //        typedef y_className Self;   \
    //        static OmClassSchema* ms_pNdbClassSchema;   \
    //        virtual const GetClassSchema() const override    \
    //        {   \
    //            return ms_pNdbClassSchema;  \
    //        }   \
    //        FORCE_INLINE static OmClassSchema* GetStaticClassSchema()   \
    //        {   \
    //            return ms_pNdbClassSchema;  \
    //        }   \
    //        static OwnerPtr<DbObject> CreateDbObject();     \
    //        void InitObject();                              \
    //        y_className();                                  \
    //        static void AddNdbSchema()                      \
    //        {   \
    //            NDB_CHECK_RELATIONSHIP(y_ns_prefix::y_className,y_parentName);  \
    //            NDB_CHECK_FULL_NAMESPACE(y_ns_prefix::y_className);             \
    //            NDB_CHECK_FULL_NAMESPACE(y_parentName);                          \
    //            ms_pNdbClassSchema=OmClassSchemaLibraryOfCode::GetSchemaLibraryOfCodeInstance()->CreateClassSchema( \
    //                Super::GetClassId(),GetClassId(),WSTRING_OF(y_parentName),WSTRING_OF(y_className),y_className::CreateDbObject,y_moduleNsPrefix::MODULE_ID,TR_MODULE_NAME,sizeof(y_className)    \
    //                );  \

#define MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)    \
        public: \
            static const ClassId& GetClassId()  \
            {   \
                static ClassId s_classId(WSTRING_OF(y_classId)); return s_classId;  \
            }   \
            typedef y_parentName Super; \
            typedef y_className Self;   \
            static OwnerPtr<DbObject> CreateDbObject();     \
            void InitObject();                              \
            y_className();                                  \
            static OmClassSchema* m_pClassSchema;           \
            virtual const OmClassSchema* GetClassSchema() const override { return m_pClassSchema; }   \
            static void AddNdbSchema()                      \
            {   \
                NDB_CHECK_RELATIONSHIP(y_className,y_parentName);                \
                NDB_CHECK_FULL_NAMESPACE(y_className);                           \
                NDB_CHECK_FULL_NAMESPACE(y_parentName);                          \
                m_pClassSchema=OmClassSchemaManager::GetInstance()->CreateOrGetClassSchema(GetClassId());       \



    ////class guid和继承关系定义
    ////y_ns_prefix:JackC  y_className:child class y_parentName:parent class. y_moduleNsPrefix:jackcelement
    //#define IDB_MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)  \
    //    public:                     \
    //        virtual Guid GetClassGuid() const override;     \
    //        virtual ISerializeBehavior* GetSerializeBehavior() cosnt override;  \
    //        virtual ICloneBehavior* GetCloneBehavior() const override;          \
    //        MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)


    //class guid和继承关系定义
    //y_ns_prefix:JackC  y_className:child class y_parentName:parent class. y_moduleNsPrefix:jackcelement
#define IDB_MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)  \
        public:                     \
            MEMBER_DATA_BEGIN_IMP(y_ns_prefix,y_className,y_parentName,y_classId,y_moduleNsPrefix)



#define MEMBER_DATA_BEGIN(NS_PREFIX, CLASS_NAME, PARENT_NAME, CLASS_ID, MODULE_NS_PREFIX)               \
        DEFINE_CAST_DERIVED(CLASS_NAME, PARENT_NAME); \
        MEMBER_DATA_BEGIN_IMP(NS_PREFIX, CLASS_NAME, PARENT_NAME, CLASS_ID, MODULE_NS_PREFIX)

#define DBOBJECT_MEMBER_DATA_BEGIN_AND_QUICK_CAST_FROM(NS_PREFIX, CLASS_NAME, PARENT_NAME, CLASS_ID, MODULE_NS_PREFIX, QUICK_CAST_SOURCE)\
        DEFINE_CAST_DERIVED2(CLASS_NAME,PARENT_NAME, QUICK_CAST_SOURCE); \
        IDB_MEMBER_DATA_BEGIN_IMP(NS_PREFIX, CLASS_NAME, PARENT_NAME, CLASS_ID, MODULE_NS_PREFIX)\



#define DBOBJECT_MEMBER_DATA_END };

#define MEMBER_DATA_END };


/*
    C++的new 包含了三个步骤：
        假如T代表一个类型（T可以表示内置类型，自定义类型，指针类型）
        对于语句T* p=new T(value)的执行过程包含：
           调用void* operator new （std::size_t size)函数，尝试分配空间，
           执行类型T的构造函数
           将void*指针转化为T类型指针
        等价伪代码为：
            void* ptr=T::operator new(sizeof(T));
            T::T(ptr,value);
            return (T*)ptr;
      operator new (std::size_t) 为想申请的字节数

      void* memset(void* s,int c,size_t n);  s为指向要填充的内存块，  c为要设置的值   n为被设置该值的字符数  返回类型是一个指向存储区s的指针
 */


#define MEMBER_DATA_DEFINE(CLASSNAME) \
            OmClassSchema* CLASSNAME::m_pClassSchema = nullptr; \
            class Ndb##CLASSNAME##AddSchema {    \
            public:                                                        \
                Ndb##CLASSNAME##AddSchema();                               \
            };                                                             \
            static Ndb##CLASSNAME##AddSchema _##Ndb##CLASSNAME##AddSchema; \
            Ndb##CLASSNAME##AddSchema::Ndb##CLASSNAME##AddSchema()         \
            {   \
                CLASSNAME::AddNdbSchema();     \
            } \
            OwnerPtr<DbObject> CLASSNAME::CreateDbObject()                                                                  \
            {   \
                void *pMem=::operator new (sizeof(CLASSNAME));                                                              \
                if(pMem)    \
                {   \
                    memset(pMem,'\0',sizeof(CLASSNAME));                                                                    \
                    new(pMem) CLASSNAME();                                                                                  \
                }   \
                CLASSNAME *pObj=(CLASSNAME*)pMem;                                                                           \
                return OwnerPtr<CLASSNAME>(pObj);                                                                           \
            }\
            CLASSNAME::CLASSNAME(){ InitObject();}                                                                          \
            void CLASSNAME::InitObject()


#define DBOBJECT_MEMBER_DATA_DEFINE(CLASSNAME) \
        MEMBER_DATA_DEFINE(CLASSNAME)
