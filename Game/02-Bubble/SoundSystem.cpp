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

void SoundSystem::PlayBackgroundMusic(BackgroundMusic i_backgroundSound)
{
	if (i_backgroundSound != m_currentBackgroundSound)
	{
		m_currentBackgroundSound = i_backgroundSound;
		m_soundEngine->stopAllSounds();
		switch (i_backgroundSound)
		{
		case BackgroundMusic::MainMenu:
			m_soundEngine->play2D("sounds/background/Calm_1.mp3",true);
			break;
		case BackgroundMusic::CreditsScreen:
			m_soundEngine->play2D("sounds/background/Cat.mp3",true);
			break;
		case BackgroundMusic::WinScreen:
			m_soundEngine->play2D("sounds/background/piano2.mp3",true);
			break;
		case BackgroundMusic::Cave:
			m_soundEngine->play2D("sounds/background/Blocks.mp3",true);
			m_soundEngine->setSoundVolume(0.5);
			break;
		case BackgroundMusic::Nether:
			m_soundEngine->play2D("sounds/background/Pigstep.mp3",true);
			break;
		case BackgroundMusic::End:
			m_soundEngine->play2D("sounds/background/creative4.mp3",true);
			break;
		}
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
	case GameplaySounds::LevelMoved:
		break;
	}
}

void SoundSystem::PlayMenuSounds(MenuSounds i_menuSounds)
{
	switch (i_menuSounds)
	{
	case MenuSounds::ButtonPressed:
		m_soundEngine->play2D("sounds/menuSounds/Click.ogg");
		break;

	}
}
}
}