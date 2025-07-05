#include "IApplication.h"

using namespace JackC;

IApplication* IApplication::Get()
{
    IAppFactory* pAppFactory = IAppFactory::Get();
    if (nullptr == pAppFactory)
    {
        return nullptr;
    }
    return pAppFactory->GetApplication();
}

IApplication::~IApplication()
{
}