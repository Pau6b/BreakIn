#pragma once
#include <irrKlang.h>
#include "ik_ISoundEngine.h"
#include "Sounds.h"

namespace game
{
namespace sound
{
class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	void PlayBackgroundMusic(BackgroundMusic i_backgroundSound);
	void PlayGameplaySounds(GameplaySounds i_gameplaySounds);
	void PlayMenuSounds(MenuSounds i_menuSounds);

private:
	irrklang::ISoundEngine* m_soundEngine;
	BackgroundMusic m_currentBackgroundSound;
};
}
}