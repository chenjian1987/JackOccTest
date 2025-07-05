#pragma once
#include "ElementDefine.h"
#include "JDevService.h"

#include "OmPropertySchema.h"
#include "OwnerPtr.h"
#include "ClassId.h"
#include "OmTypeTraits.h"

#pragma warning(push)
#pragma warning(disable:4100)

namespace JackC
{

    class JACKC_ELEMENT_EXPORT OmClassSchema
    {
    public:
        OmClassSchema(UInt32 index);
        virtual ~OmClassSchema(){}


        OmClassSchema(const OmClassSchema& another) = delete;
        OmClassSchema(OmClassSchema&& anthor) = delete;

        OmClassSchema& operator = (const OmClassSchema & another) = delete;
        OmClassSchema& operator=(OmClassSchema&& another) = delete;

    public:
        /// <summary>
        /// 获取类的Index索引
        /// </summary>
        /// <returns></returns>
        UInt32 GetIndex() const { return m_index; }

        ClassId* GetClassId();


    private:    
        /// <summary>
        /// 类的Index索引号
        /// </summary>
        UInt32                         m_index;

    };



}