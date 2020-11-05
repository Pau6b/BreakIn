#pragma once
#include <string>

namespace game
{
	class Log
	{
	public:
		static void BreakIfImpl(bool i_doBreak, std::string i_error);
	};
}


#define BreakIf(...) game::Log::BreakIfImpl(__VA_ARGS__);
