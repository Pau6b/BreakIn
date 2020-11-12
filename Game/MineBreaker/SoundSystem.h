#pragma once
#include <irrKlang.h>
#include "ik_ISoundEngine.h"
#include "Sounds.h"
#include <map>

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
	irrklang::ISoundEngine* m_backgroundEngine;
	irrklang::ISoundEngine* m_gameplayEngine;
	irrklang::ISoundEngine* m_menuSoundEngine;
	BackgroundMusic m_currentBackgroundSound;
	std::map<BackgroundMusic, irrklang::ISoundSource*> m_backgroundSounds;
	irrklang::ISoundSource* m_fightSound;
};
}
}