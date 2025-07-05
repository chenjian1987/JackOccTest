#include "IDebugWarningDisplayer.h"
#include "JAssert.h"
#include "JNew.h"
#include "Singleton.h"

#include "EnableCompileWarning_The_LAST_IncludeInCpp.h"


using namespace JackC;

namespace
{
    class DebugWarningAssertDisplayer : public IDebugWarningDisplayer
    {
        virtual bool Display(const std::string& fileName,
            int lineNumber,
            const std::string& functionName,
            const std::wstring& text,
            const std::wstring& name,
            const std::wstring& date) override
        {
            std::wstring wfileName;
            wfileName.assign(fileName.begin(), fileName.end());
            std::wstring wfunctionName;
            wfunctionName.assign(functionName.begin(), functionName.end());

            std::wstring message =
                L"Function Name: " + wfunctionName + L",\nText: " + text + L",\nName: " + name + L",\nDate: " + date;

            JACK_DBG_WARN(message, wfileName, lineNumber);
            return true;
        }

        virtual bool ShouldAssertInMultithread() const override
        {
            return true;
        }
    };
}

using IDebugWarningDisplayerSingleton 
    = Singleton<IDebugWarningDisplayer, DebugWarningAssertDisplayer>;

IDebugWarningDisplayer* IDebugWarningDisplayer::Get()
{
    return IDebugWarningDisplayerSingleton::Get().GetObject();
}

bool IDebugWarningDisplayer::SwitchTo(OwnerPtr<IDebugWarningDisplayer> displayer)
{
    IDebugWarningDisplayerSingleton::Get().SetCurrent(TransferOwnership(displayer));
    return true;
}

OwnerPtr<IDebugWarningDisplayer> IDebugWarningDisplayer::CreateAssertDisplayer()
{
    return NEW_AS_OWNER_PTR(DebugWarningAssertDisplayer);
}
