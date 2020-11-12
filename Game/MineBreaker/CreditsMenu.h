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

		class CreditsMenu : public core::Scene
		{
		public:
			CreditsMenu(sound::SoundSystem& i_soundSystem);
			void init() override;
			void update(int i_deltaTime) override;
			void render() override;
			std::pair<core::Scene::SceneResult, uint32_t> GetSceneResult() override;

			void OnMouseButtonReleased(int32_t i_button) override;

		private:
			void UpdateButtonMousePositions();
			struct ButtonAction
			{
				ButtonAction(std::unique_ptr<Button> i_button, core::Scene::SceneResult i_sceneAction);
				std::unique_ptr<Button> button;
				core::Scene::SceneResult sceneAction;
			};

			core::Scene::SceneResult m_currentResult = core::Scene::SceneResult::NotFinished;
			std::unique_ptr<visuals::ShaderProgram> m_shaderProgram;
			glm::mat4 m_projection;

			std::unique_ptr<visuals::Sprite> m_background;
			std::unique_ptr<visuals::Sprite> m_credits;
			std::vector<ButtonAction> m_buttons;

			sound::SoundSystem& m_soundSystem;
		};
	}
}