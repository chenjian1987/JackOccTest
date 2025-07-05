#include "DebugMessageMgrImpl.h"
#include "BasicConfigUtils.h"
#include <iostream>
#include <algorithm>
#include "JNew.h"
#include <sstream>
#include "IDebugWarningDisplayer.h"
#include <thread>
#ifdef _WINDOWS
#include <windows.h>
#include <TlHelp32.h>
#endif

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"
#include "LogManager.h"
#include "JAssert.h"

using namespace JackC;

JackC::OwnerPtr<DebugMessageMgrImpl> DebugMessageMgrImpl::ms_upTheInstance;

DebugMessageMgrImpl::DebugMessageMgrImpl()
    : m_bWillQuitApplication(false)
    , m_bIsShowingDebugWarn(false)
{
}

DebugMessageMgrImpl::~DebugMessageMgrImpl()
{
}

const DebugMessageMgrImpl* DebugMessageMgrImpl::GetDebugMessageMgrImpl()
{
    if (!ms_upTheInstance)
    {
        ms_upTheInstance = NEW_AS_OWNER_PTR(DebugMessageMgrImpl);
    }
    return ms_upTheInstance.get();
}

DebugMessageMgrImpl* DebugMessageMgrImpl::GetDebugMessageMgrImplFW()
{
    if (!ms_upTheInstance)
    {
        ms_upTheInstance = NEW_AS_OWNER_PTR(DebugMessageMgrImpl);
    }
    return ms_upTheInstance.get();
}

void DebugMessageMgrImpl::ShowDebugWarn(const std::string& fileName, 
    int lineNumber, 
    const std::string& functionName, 
    const std::wstring& text, 
    const std::wstring& name, 
    const std::wstring& date) const
{
    if (!BasicConfigUtils::DebugMessageEnabled())
    {
        return;
    }

    std::wostringstream woss;

    woss << L"[" << std::this_thread::get_id() << L"]\n\r\n\r";
    woss << L"File: " << fileName.c_str() << L"\n\rLine: " << lineNumber << L"\n\rFunction: " << functionName.c_str() << L"\n\r\n\r";

    // 用于忽略将来相同信息
    std::wstring messageLocation = woss.str();
    
    // 是否以记录为不再显示
    if(m_ignoredMessageLocations.find(messageLocation) != m_ignoredMessageLocations.end())
    {
        return;
    }

    woss << L"Message: " << text << L"\n\rName: " << name << L"\n\rDate: " << date;

    // 先输出到日志中
    // 因为Singleton的初始化顺序有可能产生嵌套，这里防止LogMgr还未初始化完成却被嵌套调用的情况。
    if(LogManager::IsReady())
    {
        LogManager::CreateOrGetForWrite()->WriteLine(LogTypeOfLogManager::DEBUG_WARN, woss.str(), true);
    }

    std::for_each(m_preShowListeners.begin(), m_preShowListeners.end(), [](std::function<void()> listener) {
        listener();
    });

    if (ShouldShowDebugWarnDialog())
    {
        m_bIsShowingDebugWarn = true;
    
        IDebugWarningDisplayer* pDisplayer = IDebugWarningDisplayer::Get();
        if (pDisplayer != nullptr)
        {
            bool isDisplayAgain = pDisplayer->Display(fileName, lineNumber, functionName, text, name, date);
            m_bIsShowingDebugWarn = false;
            if (!isDisplayAgain)
            {
                m_ignoredMessageLocations.insert(messageLocation);
            }     
        }
    }
    else if (BasicConfigUtils::IsRunAsService())
    {
        m_bIsShowingDebugWarn = false;
        m_ignoredMessageLocations.insert(messageLocation);
    }
    else if (!IsMainThread())
    {
        m_bIsShowingDebugWarn = false;
        m_ignoredMessageLocations.insert(messageLocation);
    }
    else
    {
        m_bIsShowingDebugWarn = false;
        m_ignoredMessageLocations.insert(messageLocation);
        JACK_ASSERT(false && "Unexpected errors occured.");
    }

    std::for_each(m_postShowListeners.begin(), m_postShowListeners.end(), [](std::function<void()> listener) {
        listener();
    });
}

void DebugMessageMgrImpl::RegisterAsPreShowListener(std::function<void()> listener)
{
    m_preShowListeners.push_back(listener);
}

void DebugMessageMgrImpl::RegisterAsPostShowListener(std::function<void()> listener)
{
    m_postShowListeners.push_back(listener);
}

void DebugMessageMgrImpl::Dump() const
{
    /*std::wostringstream oss;
    oss << L"Start dumping DebugMessageMgr...";
    DUMP(oss.str());
    {
        DumpIntentSetter dis(2);

        oss.str(L"");
        oss << L"Suppressed Warnings: ";
        DUMP(oss.str());

        for(std::set<std::wstring>::iterator it = m_ignoredMessageLocations.begin(); it != m_ignoredMessageLocations.end(); ++it)
        {
            DumpIntentSetter subDis(2);
            DUMP(*it);
        }
    }*/
}


void DebugMessageMgrImpl::SetWillQuitApplication( bool bWillQuitApplication )
{
    m_bWillQuitApplication = bWillQuitApplication;
}


bool DebugMessageMgrImpl::IsMainThread() const
{
#ifdef _WINDOWS
    DWORD threadId = GetCurrentThreadId();
    // 系统所有线程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return true;

    DWORD processId = GetCurrentProcessId();
    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);
    // 第一个线程
    if (Thread32First(hSnapshot, &threadEntry))
    {
        do
        {
            // 认为找到的第一个该进程的线程为主线程
            if (processId == threadEntry.th32OwnerProcessID)
            {
                threadId = threadEntry.th32ThreadID;
                break;
            }
        } while (Thread32Next(hSnapshot, &threadEntry));
    }
    CloseHandle(hSnapshot);
    return threadId == GetCurrentThreadId();
#else
    return false;
#endif
}

bool DebugMessageMgrImpl::ShouldShowDebugWarnDialog() const
{
    IDebugWarningDisplayer* pDisplayer = IDebugWarningDisplayer::Get();
    if (pDisplayer != nullptr)
    {
        return !m_bWillQuitApplication && 
            (IsMainThread()|| !pDisplayer->ShouldAssertInMultithread()) && 
            !BasicConfigUtils::IsRunAsService();
    }
    return false;
}

