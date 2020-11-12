#include "SoundSystem.h"

namespace game
{
namespace sound
{

SoundSystem::SoundSystem()
	: m_backgroundEngine ( irrklang::createIrrKlangDevice() )
	, m_gameplayEngine ( irrklang::createIrrKlangDevice() )
	, m_menuSoundEngine( irrklang::createIrrKlangDevice() )
{
	if (!m_backgroundEngine)
	{
		return;
	}

}

SoundSystem::~SoundSystem()
{
	m_backgroundEngine->drop();
}

void SoundSystem::PlayBackgroundMusic(BackgroundMusic i_backgroundSound)
{
	if (i_backgroundSound != m_currentBackgroundSound)
	{
		m_currentBackgroundSound = i_backgroundSound;
		m_backgroundEngine->stopAllSounds();
		m_gameplayEngine->stopAllSounds();
		switch (i_backgroundSound)
		{
		case BackgroundMusic::MainMenu:
			m_backgroundEngine->play2D("sounds/background/Calm_1.mp3",true);
			break;
		case BackgroundMusic::CreditsScreen:
			m_backgroundEngine->play2D("sounds/background/Cat.mp3",true);
			break;
		case BackgroundMusic::WinScreen:
			m_backgroundEngine->play2D("sounds/background/piano2.mp3",true);
			break;
		case BackgroundMusic::Cave:
			m_backgroundEngine->play2D("sounds/background/Blocks.mp3",true);
			m_backgroundEngine->setSoundVolume(0.3);
			break;
		case BackgroundMusic::Nether:
			m_backgroundEngine->play2D("sounds/background/Pigstep.mp3",true);
			break;
		case BackgroundMusic::End:
			m_backgroundEngine->play2D("sounds/background/creative4.mp3",true);
			break;
		}
	}
}

void SoundSystem::PlayGameplaySounds(GameplaySounds i_gameplaySounds)
{
	switch (i_gameplaySounds)
	{
	case GameplaySounds::BlockHit:
		m_gameplayEngine->play2D("sounds/gameplaySounds/Wood.mp3", false);
		break;
	case GameplaySounds::BlockBroken:
		m_gameplayEngine->play2D("sounds/gameplaySounds/block.mp3", false);
		break;
	case GameplaySounds::BallStaticBlockCollision:
		m_gameplayEngine->play2D("sounds/gameplaySounds/hitblock.mp3", false);
		break;
	case GameplaySounds::BallBarCollision:
		m_gameplayEngine->play2D("sounds/gameplaySounds/slime.mp3", false);
		break;
	case GameplaySounds::BallCoin:
		m_gameplayEngine->play2D("sounds/gameplaySounds/orb.mp3", false);
		break;
	case GameplaySounds::BallKey:
		m_gameplayEngine->play2D("sounds/gameplaySounds/achievement.mp3", false);
		break;
	case GameplaySounds::LiveLost:
		m_gameplayEngine->play2D("sounds/gameplaySounds/classic_hurt.mp3", false);
		break;
	case GameplaySounds::Died:
		m_backgroundEngine->stopAllSounds();
		m_gameplayEngine->play2D("sounds/gameplaySounds/gta.mp3", false);
		break;
	case GameplaySounds::AlarmAcivated:
		m_backgroundEngine->stopAllSounds();
		m_gameplayEngine->play2D("sounds/gameplaySounds/fight.mp3", true);
		m_currentBackgroundSound = BackgroundMusic::Fight;
		break;
	case GameplaySounds::Portal:
		m_gameplayEngine->play2D("sounds/gameplaySounds/vwoop.mp3", false);
		break;
	}
}

void SoundSystem::PlayMenuSounds(MenuSounds i_menuSounds)
{
	switch (i_menuSounds)
	{
	case MenuSounds::ButtonPressed:
		m_menuSoundEngine->play2D("sounds/menuSounds/Click.ogg");
		break;

	}
}
}
}