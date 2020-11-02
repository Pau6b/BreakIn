#include "SoundSystem.h"

namespace game
{
namespace sound
{

SoundSystem::SoundSystem()
	: m_soundEngine ( irrklang::createIrrKlangDevice() )
{
	if (!m_soundEngine)
	{
		return;
	}

}

SoundSystem::~SoundSystem()
{
	m_soundEngine->drop();
}

void SoundSystem::PlayBackgroundSounds(BackgroundSounds i_backgroundSounds)
{
	switch (i_backgroundSounds)
	{
	case BackgroundSounds::MenuMusic:
		break;
	case BackgroundSounds::CaveMusic:
		break;
	case BackgroundSounds::NetherMusic:
		break;
	case BackgroundSounds::EndMusic:
		break;
	}
}

void SoundSystem::PlayGameplaySounds(GameplaySounds i_gameplaySounds)
{
	switch (i_gameplaySounds)
	{
	case GameplaySounds::BlockHit:
		break;
	case GameplaySounds::BlockBroken:
		break;
	case GameplaySounds::BallStaticBlockCollision:
		break;
	case GameplaySounds::BallBarCollision:
		break;
	case GameplaySounds::BallCoin:
		break;
	case GameplaySounds::BallKey:
		break;
	case GameplaySounds::LiveLost:
		break;
	case GameplaySounds::Died:
		break;
	case GameplaySounds::AlarmAcivated:
		break;
	}
}

void SoundSystem::PlayMenuSounds(MenuSounds i_menuSounds)
{
	switch (i_menuSounds)
	{
	case MenuSounds::ButtonPressed:
		break;

	}
}

}
}