#pragma once

namespace game
{
namespace sound
{
enum class BackgroundSounds
{
	MenuMusic,
	CaveMusic,
	NetherMusic,
	EndMusic
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
	AlarmAcivated
};

enum class MenuSounds
{
	ButtonPressed
};
}
}