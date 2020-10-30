#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "ShaderProgram.h"

namespace game
{
namespace gui
{
class MainScreenScene : public core::Scene
{
public:
	void init() override;
	void update(int i_deltaTime) override;
	void render() override;
	std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;

	void OnMouseButtonReleased(int32_t i_button) override;

private:

	//#pau_todo move this to a button class or something like this if needed
	struct ButtonInfo
	{
		int32_t xPos;
		int32_t yPos;
		int32_t xWidth;
		int32_t yWidth;
	};

	static inline bool IsMouseIsInButton(const ButtonInfo& i_buttonInfo, const std::pair<int32_t, int32_t>& i_mousePosition);
	
	core::Scene::SceneResult m_currentResult = core::Scene::SceneResult::NotFinished;
	std::unique_ptr<visuals::Sprite> m_buttonText;
	std::unique_ptr<visuals::ShaderProgram> m_shaderProgram;
	glm::mat4 m_projection;
	
	ButtonInfo m_playButtonInfo;
};
}
}