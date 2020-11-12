#pragma once

namespace game
{
namespace sound
{
enum class BackgroundMusic
{
	MainMenu,
	CreditsScreen,
	Cave,
	Nether,
	End,
	WinScreen,
	Fight
};

enum class GameplaySounds
{
	BlockHit,
	BlockBroken,
	BallStaticBlockCollision,
	BallBarCollision,
	BallCoin,
	BallKey,
	LiveLost,
	Died,
	AlarmAcivated,
	LevelMoved,
	Portal
};

enum class MenuSounds
{
	ButtonPressed
};
}
}