#pragma once

#include "ElementDefine.h"
#include "JDevService.h"

namespace JackC
{
    class IDbObjectSerialize;
    class IIOStream;
    class OmClassSchema;
    class ElementId;

    enum class EnumIndexForamt :int32_t
    {
        Invalid=0,
        Uint8=1,
        Uint16=2,
        Uint32=3
    };

    class  JACKC_ELEMENT_EXPORT OmClassIdIndexUtils
    {
    public:
        /// <summary>
        /// 为Element在尾部序列化其用到的ClassId和Index表
        /// </summary>
        /// <param name="pObjArchive"></param>
        /// <param name="currentObjectLen"></param>
        /// <returns></returns>
        static int32_t WriteClassIdToIndexMapForElement(IDbObjectSerialize* pObjArchive, int64_t currentObjectLen);

        /// <summary>
        /// 为Object序列化BodyMeta中的Index或者Guid
        /// </summary>
        /// <param name="ioStream"></param>
        /// <param name="pClassSchema"></param>
        /// <param name="pObjArchive"></param>
        /// <returns></returns>
        static int64_t SerializeIndexOrGuidForBodyMeta(IIOStream& ioStream, const OmClassSchema* pClassSchema, IDbObjectSerialize* pObjArchive);
    };

}