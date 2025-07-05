#include "DebugMessage.h"
#include "JNew.h"
#include "DebugMessageMgrImpl.h"

using namespace JackC;

namespace
{
    static JackC::OwnerPtr<DebugMessageMgr> ms_upGlobalDebugMessageMgrInstance;
    static DebugMessageMgrImpl* m_debugMessageMgrImpl;
}


DebugMessageMgr::DebugMessageMgr()
{
    m_debugMessageMgrImpl = DebugMessageMgrImpl::GetDebugMessageMgrImplFW();
}

DebugMessageMgr::~DebugMessageMgr()
{
}

const DebugMessageMgr* DebugMessageMgr::GetDebugMessageMgr()
{
    if (!ms_upGlobalDebugMessageMgrInstance)
    {
        ms_upGlobalDebugMessageMgrInstance = NEW_AS_OWNER_PTR(DebugMessageMgr);
    }
    return ms_upGlobalDebugMessageMgrInstance.get();
}

DebugMessageMgr* DebugMessageMgr::GetDebugMessageMgrFW()
{
    if (!ms_upGlobalDebugMessageMgrInstance)
    {
        ms_upGlobalDebugMessageMgrInstance = NEW_AS_OWNER_PTR(DebugMessageMgr);
    }
    return ms_upGlobalDebugMessageMgrInstance.get();
}

void DebugMessageMgr::ShowDebugWarn(
    const std::string& fileName, 
    int lineNumber, 
    const std::string& functionName, 
    const std::wstring& text, 
    const std::wstring& name, 
    const std::wstring& date) const
{
    if (m_debugMessageMgrImpl == nullptr)
    {
        return;
    }

    m_debugMessageMgrImpl->ShowDebugWarn(fileName, lineNumber, functionName, text, name, date);
}

void DebugMessageMgr::RegisterAsPreShowListener(std::function<void()> listener)
{
    if (m_debugMessageMgrImpl == nullptr)
        return;

    m_debugMessageMgrImpl->RegisterAsPreShowListener(listener);
}

void DebugMessageMgr::RegisterAsPostShowListener(std::function<void()> listener)
{
    if (m_debugMessageMgrImpl == nullptr)
        return;

    m_debugMessageMgrImpl->RegisterAsPostShowListener(listener);
}

void DebugMessageMgr::Dump() const
{
    if (m_debugMessageMgrImpl == nullptr)
        return;

    m_debugMessageMgrImpl->Dump();
}

void DebugMessageMgr::SetWillQuitApplication( bool bWillQuitApplication )
{
    if (m_debugMessageMgrImpl == nullptr)
        return;

    m_debugMessageMgrImpl->SetWillQuitApplication(bWillQuitApplication);
}

bool DebugMessageMgr::IsShowingDebugWarn() const
{
    if (m_debugMessageMgrImpl == nullptr)
        return false;

    return m_debugMessageMgrImpl->IsShowingDebugWarn();
}