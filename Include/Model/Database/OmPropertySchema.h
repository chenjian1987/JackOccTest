#pragma once

#include "ElementDefine.h"
#include "OmPropertySchemaType.h"
#include "JTypeDefs.h"

namespace JackC
{
    class IIOStream;
    class UniqueWString;

    class JACKC_ELEMENT_EXPORT OmPropertySchema
    {
    public:
        OmPropertySchema(OmPropertySchemaType propertySchemaType, IntOrFloatType intOrFloatType, IntBaseType intBaseType, const UniqueWString* pPropertyName, const UniqueWString* pPropertyType,
            uint32_t offset, uint32_t propertySize, int32_t propertyBehaviorTypes );
        OmPropertySchema();

    public:
        std::wstring ToWString() const;

        void SetPropertyName(const UniqueWString* propertyName);
        const UniqueWString* GetPropertyName() const;

        void SetPropertyType(const UniqueWString* propertyType);
        const UniqueWString* GetPropertyType() const;

        void SetOffset(uint32_t offset);
        uint32_t GetOffset() const;

        void SetPropertySize(unsigned int propertySize);
        uint32_t GetPropertySize() const;

        OmPropertySchemaType GetPropertySchemaType() const;
        void SetPropertySchemaType(OmPropertySchemaType propertySchemaType);

        IntOrFloatType GetIntOrFloatType() const;
        void SetIntOrFloatType(IntOrFloatType intOrFloatType);

        IntBaseType GetIntBaseType() const;
        void SetIntBaseType(IntBaseType intBaseType);

        int32_t GetPropertyBehaviourTypes() const;
        void SetPropertyBehaviourTypes(int32_t propertyBehaviourTypes);

        void Serialize(IIOStream& ioStream) const;
        void Unserialize(IIOStream& ioStream);
        bool Compare(const OmPropertySchema* pPropertySchema) const;

    protected:
        friend class OmClassSchemaLibraryUtils;

        /// <summary>
        /// 属性值schema类型
        /// </summary>
        OmPropertySchemaType m_propertySchemaType;
        /// <summary>
        /// 属性值整型，浮点类型
        /// </summary>
        IntOrFloatType m_intOrFloatType;
        /// <summary>
        /// 进制类型
        /// </summary>
        IntBaseType m_intBaseType;
        /// <summary>
        /// 属性值名称
        /// </summary>
        const UniqueWString* m_usPropertyName;
        /// <summary>
        /// 属性值类型
        /// </summary>
        const UniqueWString* m_usPropertyType;

        /// <summary>
        /// 属性值偏移大小
        /// </summary>
        uint32_t m_offset;
        /// <summary>
        /// 属性值大小
        /// </summary>
        uint32_t m_propertySize;
        /// <summary>
        /// 属性行为类型
        /// </summary>
        int32_t m_propertyBehaviorTypes;
    };


    template <class T>
    inline const T* GetPropertyAddress(const void* pObject, const OmPropertySchema* propertySchema)
    {
        return reinterpret_cast<const T*>((char*)pObject + propertySchema->GetOffset());
    }

    template <class T>
    inline T* GetPropertyAddressFW(void* pObject, const OmPropertySchema& propertySchema)
    {
        return reinterpret_cast<T*>((char*)pObject + propertySchema.GetOffset());
    }
}