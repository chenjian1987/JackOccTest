#pragma once

#include "OmPropertySchema.h"
#include "OwnerPtr.h"
#include "ClassId.h"
#include "OmTypeTraits.h"
#include "OmClassSchema.h"




#pragma warning(push)
#pragma warning(disable:4100)

namespace JackC
{

    class JACKC_ELEMENT_EXPORT  OmClassSchemaManager
    {
    public:
        OmClassSchemaManager();
        virtual ~OmClassSchemaManager() {}

    public:
        static OmClassSchemaManager* GetInstance();

        OmClassSchema* CreateOrGetClassSchema(const ClassId& classId);
        /// <summary>
        /// 获取class的schema数量
        /// </summary>
        virtual UInt32 GetClassSchemaCount() const  { return (UInt32)m_classSchemaArray.size(); }

        ClassId* GetClassId(const OmClassSchema& schema);

        std::vector<uint32_t> GetAllIndex();







    private:
        static OmClassSchemaManager* m_pInstance;

        /// <summary>
        /// 所有类schema列表
        /// </summary>
        std::vector<OwnerPtr<OmClassSchema>>              m_classSchemaArray;

        /// <summary>
        /// 类schema的索引映射
        /// </summary>
        std::unordered_map<ClassId, UInt32, ClassIdHasher> m_classId2Index;

    };



}