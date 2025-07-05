
#pragma once

#include "JDevService.h"
#include "TransferOwnership.h"

namespace JackC
{
    // 一个可切换实例对象的单例, 需要提供默认类型
    template<class BaseType, class DefaultType,
        class = typename std::enable_if<std::is_convertible<DefaultType*, BaseType*>::value, void>::type>
        class Singleton final
    {
        using InstanceType = Singleton<BaseType, DefaultType>;
    private:
        OwnerPtr<BaseType> m_opDefault;
        OwnerPtr<BaseType> m_opCurrent;

    private:
        Singleton()
            : m_opDefault(NEW_AS_OWNER_PTR(DefaultType))
            , m_opCurrent(nullptr)
        {}

    public:
        /// \brief 获取当前的单例对象
        static InstanceType& Get()
        {
            static InstanceType singleton;
            return singleton;
        }

    public:
        /// \brief 修改当前生效的单例实体
        /// \param object 新的待设置实体
        void SetCurrent(OwnerPtr<BaseType> object)
        {
            m_opCurrent = TransferOwnership(object);
        }

        /// \brief 修改默认的单例实体， 在当前没有生效的单例实体时， 启用默认的单例实体
        /// \param object 新的待设置实体
        void SetDefault(OwnerPtr<BaseType> object)
        {
            m_opDefault = TransferOwnership(object);
        }

        /// \brief 获当前启用的单例实体
        /// \return 当前启用的单例实体
        BaseType* GetObject()
        {
            return m_opCurrent ? m_opCurrent.get() : m_opDefault.get();
        }
    };

}

