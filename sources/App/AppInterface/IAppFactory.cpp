#include "IAppFactory.h"
#include "LogManager.h"


using namespace JackC;




IAppFactory* IAppFactory::m_pFactory = nullptr;

IAppFactory* IAppFactory::Get()
{
    if (!m_pFactory)
    {
        FYI(L"Application: Application Factory is null");
    }
    return m_pFactory;
}