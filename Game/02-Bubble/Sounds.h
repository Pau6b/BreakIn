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
	WinScreen
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
	LevelMoved
};

enum class MenuSounds
{
	ButtonPressed
};
}
}