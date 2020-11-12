#pragma once
#include <memory>
#include "Scene.h"
#include "Sprite.h"
#include "ShaderProgram.h"
#include "Button.h"
#include <string>

namespace game
{
	namespace sound
	{
		class SoundSystem;
	}

	namespace gui
	{
		class Text;
	}
}

namespace game
{
	namespace gui
	{
		class Button;

		class PasswordsMenu : public core::Scene
		{
		public:
			PasswordsMenu(sound::SoundSystem& i_soundSystem, const std::map<std::string, uint32_t>& i_passwords);
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

			std::vector<ButtonAction> m_buttons;
			std::unique_ptr<visuals::Sprite> m_background;
			std::unique_ptr<visuals::Sprite> m_limit;
			sound::SoundSystem& m_soundSystem;

			std::unique_ptr<gui::Text> m_text;
			uint32_t m_finalMap;

			float m_timeElapsed;
			bool m_printLimit;
			char m_pressKey;
			std::string m_userText;

			const std::map<std::string, uint32_t> m_passwords;

		};
	}
}