#pragma once
#ifdef __cplusplus
#include "JCassert.h"
#else
#include "Jack_assert.h"
#endif

#include <algorithm>
#include <functional>
#include <vector>
#include "JCommonDefine.h"

namespace JackC
{
	//∂œ—‘π§æﬂ¿‡
	class AssertHelper
	{
	private:
		AssertHelper() {}
		virtual ~AssertHelper() {}
	public:
		JACKC_COMMON_INTERFACE_EXPORT static AssertHelper* GetInstance();
		JACKC_COMMON_INTERFACE_EXPORT void RegisterAsPreShowListener(std::function<void()> listener);
		JACKC_COMMON_INTERFACE_EXPORT void Assert(const std::wstring& text, const std::wstring& fileName, int line) const;
	private:
		std::vector<std::function<void()>> m_preShowListeners;

	};
}
