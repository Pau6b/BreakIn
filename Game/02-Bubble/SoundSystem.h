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
	SoundSystem();
	~SoundSystem();

	void PlayBackgroundSounds(BackgroundSounds i_backgroundSounds);
	void PlayGameplaySounds(GameplaySounds i_gameplaySounds);
	void PlayMenuSounds(MenuSounds i_menuSounds);

private:
	irrklang::ISoundEngine* m_soundEngine;
};
}
}