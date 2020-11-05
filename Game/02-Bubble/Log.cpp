#include "Log.h"
#include <windows.h>
#include <debugapi.h>

namespace game
{
void Log::BreakIfImpl(bool i_doBreak, std::string i_error)
{
	if(i_doBreak)
	{
		DebugBreak();
	}
}
}