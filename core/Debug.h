#pragma once

#ifdef MYE_DEBUG

#include <iostream>
#include "Logger.h"

namespace mye
{
	namespace core
	{
		extern Logger g_debugLog;
	}
}

#define MYE_DEBUG_EVENT(msg) g_debugLog.LogEvent(msg);

#else

#define MYE_DEBUG_EVENT(msg)

#endif