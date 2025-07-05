#include "WeakReferenceable.h"
#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"

using namespace JackC;


WeakReferenceable::~WeakReferenceable(void)
{
    if (m_pProxy)
    {
        m_pProxy->Reset();
        m_pProxy = nullptr;
    }
}

WeakReferenceable::WeakReferenceable(const WeakReferenceable&)
{
    m_pProxy = nullptr;
}

WeakReferenceable::WeakReferenceable():m_pProxy(nullptr){}


WeakReferenceable& WeakReferenceable::operator=(const WeakReferenceable&)
{
    return *this;
}

WeakReferenceable::Proxy* WeakReferenceable::GetOrCreateProxy() const
{
    if (!m_pProxy)
    {
        m_pProxy = new Proxy(const_cast<WeakReferenceable*>(this));
    }
    return m_pProxy;
}