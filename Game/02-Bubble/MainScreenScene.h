#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "ShaderProgram.h"
#include "Button.h"

namespace game
{
namespace sound
{
	class SoundSystem;
}
}

namespace game
{
namespace gui
{
class Button;

class MainScreenScene : public core::Scene
{
public:
	MainScreenScene(sound::SoundSystem& i_soundSystem);
	void init() override;
	void update(int i_deltaTime) override;
	void render() override;
	std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;

	void OnMouseButtonReleased(int32_t i_button) override;

private:

	//#pau_todo move this to a button class or something like this if needed
	
	core::Scene::SceneResult m_currentResult = core::Scene::SceneResult::NotFinished;
	std::unique_ptr<visuals::ShaderProgram> m_shaderProgram;
	glm::mat4 m_projection;
	
	std::unique_ptr<Button> m_playButton;

	sound::SoundSystem& m_soundSystem;
};
}
}