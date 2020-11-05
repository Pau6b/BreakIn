#include"SoundHelpers.h"
#include "Log.h"

namespace game
{
namespace sound
{
namespace helpers
{

BackgroundMusic StringToBackgroundSound(std::string i_soundString)
{
	if (i_soundString == "MainMenu")
	{
		return BackgroundMusic::MainMenu;
	}
	else if (i_soundString == "CreditsScreen")
	{
		return BackgroundMusic::CreditsScreen;
	}
	else if (i_soundString == "Cave")
	{
		return BackgroundMusic::Cave;
	}
	else if (i_soundString == "Nether")
	{
		return BackgroundMusic::Nether;
	}
	else if (i_soundString == "End")
	{
		return BackgroundMusic::End;
	}
	else if (i_soundString == "WinScreen")
	{
		return BackgroundMusic::WinScreen;
	}
	BreakIf(true, "No sound found");
	return BackgroundMusic::MainMenu;
}

}
}
}