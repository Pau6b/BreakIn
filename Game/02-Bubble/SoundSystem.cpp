#include "SoundSystem.h"
#include "Log.h"
#include <iostream>
#include "ik_ISoundSource.h"

namespace game
{
namespace sound
{

SoundSystem::SoundSystem()
	: m_backgroundEngine ( irrklang::createIrrKlangDevice() )
	, m_gameplayEngine ( irrklang::createIrrKlangDevice() )
	, m_menuSoundEngine( irrklang::createIrrKlangDevice() )
{
	BreakIf(!m_backgroundEngine, "Background sound engine initialization was not correct");
	BreakIf(!m_gameplayEngine, "Gameplay sound engine initialization was not correct");
	BreakIf(!m_menuSoundEngine, "Menu sound engine initialization was not correct");

	m_backgroundSounds.emplace(BackgroundMusic::MainMenu, m_backgroundEngine->addSoundSourceFromFile("sounds/background/Calm_1.mp3"));
	m_backgroundSounds.at(BackgroundMusic::MainMenu)->setDefaultVolume(0.4f);
	m_backgroundSounds.emplace(BackgroundMusic::CreditsScreen, m_backgroundEngine->addSoundSourceFromFile("sounds/background/Cat.mp3"));
	m_backgroundSounds.at(BackgroundMusic::CreditsScreen)->setDefaultVolume(0.5f);
	m_backgroundSounds.emplace(BackgroundMusic::WinScreen, m_backgroundEngine->addSoundSourceFromFile("sounds/background/piano2.mp3"));
	m_backgroundSounds.at(BackgroundMusic::WinScreen)->setDefaultVolume(0.5f);
	m_backgroundSounds.emplace(BackgroundMusic::Cave, m_backgroundEngine->addSoundSourceFromFile("sounds/background/Blocks.mp3"));
	m_backgroundSounds.at(BackgroundMusic::Cave)->setDefaultVolume(0.08f);
	m_backgroundSounds.emplace(BackgroundMusic::Nether, m_backgroundEngine->addSoundSourceFromFile("sounds/background/Pigstep.mp3"));
	m_backgroundSounds.at(BackgroundMusic::Nether)->setDefaultVolume(0.05f);
	m_backgroundSounds.emplace(BackgroundMusic::End, m_backgroundEngine->addSoundSourceFromFile("sounds/background/creative4.mp3"));
	m_backgroundSounds.at(BackgroundMusic::End)->setDefaultVolume(0.3f);
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
		m_backgroundEngine->play2D(m_backgroundSounds.at(i_backgroundSound), true);
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
		m_backgroundEngine->play2D("sounds/gameplaySounds/fight.mp3", true);
		m_currentBackgroundSound = BackgroundMusic::Fight;
		break;
	case GameplaySounds::Portal:
		m_gameplayEngine->play2D("sounds/gameplaySounds/vwoop.mp3", false);
		break;
	}
	m_gameplayEngine->setSoundVolume(0.1f);
}

void SoundSystem::PlayMenuSounds(MenuSounds i_menuSounds)
{
	switch (i_menuSounds)
	{
	case MenuSounds::ButtonPressed:
		m_menuSoundEngine->play2D("sounds/menuSounds/Click.ogg");
		break;

	}
	m_menuSoundEngine->setSoundVolume(0.1f);
}
}
}