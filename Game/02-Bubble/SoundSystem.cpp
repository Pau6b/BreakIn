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
			m_soundEngine->setSoundVolume(0.3);
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
		m_soundEngine->play2D("sounds/gameplaySounds/Wood.mp3", false);
		break;
	case GameplaySounds::BlockBroken:
		m_soundEngine->play2D("sounds/gameplaySounds/break.mp3", false);
		break;
	case GameplaySounds::BallStaticBlockCollision:
		m_soundEngine->play2D("sounds/gameplaySounds/block.mp3", false);
		break;
	case GameplaySounds::BallBarCollision:
		m_soundEngine->play2D("sounds/gameplaySounds/slime.mp3", false);
		break;
	case GameplaySounds::BallCoin:
		m_soundEngine->play2D("sounds/gameplaySounds/orb.mp3", false);
		break;
	case GameplaySounds::BallKey:
		m_soundEngine->play2D("sounds/gameplaySounds/achievement.mp3", false);
		break;
	case GameplaySounds::LiveLost:
		m_soundEngine->play2D("sounds/gameplaySounds/classic_hurt.mp3", false);
		break;
	case GameplaySounds::Died:
		m_soundEngine->stopAllSounds();
		m_soundEngine->play2D("sounds/gameplaySounds/gta.mp3", false);
		break;
	case GameplaySounds::AlarmAcivated:
		m_soundEngine->play2D("sounds/gameplaySounds/alarm.mp3", false);
		break;
	case GameplaySounds::LevelMoved:
		m_soundEngine->play2D("sounds/gameplaySounds/vwoop.mp3", false);
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