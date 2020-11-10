#include "PasswordsMenu.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SoundSystem.h"
#include "Button.h"
#include <algorithm>
#include "Text.h"

#define FONT_SIZE 21

namespace game
{
	namespace gui
	{

		PasswordsMenu::PasswordsMenu(sound::SoundSystem& i_soundSystem)
			: m_soundSystem(i_soundSystem)
			, m_timeElapsed(0)
			, m_printLimit(0)
			, m_pressKey('n')
			, m_finalMap(0)
		{

		}

		void PasswordsMenu::init()
		{
			m_shaderProgram = std::make_unique<visuals::ShaderProgram>();
			InitShaders(*m_shaderProgram, "shaders/menu.vert", "shaders/menu.frag");
			m_background = std::make_unique<visuals::Sprite>(glm::vec2(640, 480), glm::vec2(1, 1), "images/UI/Seed.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, *m_shaderProgram);
			m_limit = std::make_unique<visuals::Sprite>(glm::vec2(2, 32), glm::vec2(1, 1), "images/UI/limit_text.png", visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA, *m_shaderProgram);
			m_limit->setPosition(glm::vec2(115,164));
			std::unique_ptr<Button> controlsButton = std::make_unique<Button>(170, 380, 300, 33, *m_shaderProgram, Button::ButtonText::Back);
			m_buttons.emplace_back(std::move(controlsButton), core::Scene::SceneResult::GoToMainMenu);
			m_projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
			m_text = std::make_unique<gui::Text>("fonts/Minecraft-Regular.otf", FONT_SIZE);
		}

		void PasswordsMenu::update(int i_deltaTime)
		{
			//13 is enter
			m_timeElapsed += float(i_deltaTime)/1000;
			if (core::Game::instance().getKey(13))
			{
				bool correctKey = false;
				if (m_userText == "111") 
				{
					m_finalMap = 1;
					correctKey = true;
				}
				else if (m_userText == "222")
				{
					m_finalMap = 2;
					correctKey = true;
				}
				else if (m_userText == "333")
				{
					m_finalMap = 3;
					correctKey = true;
				}
				else
				{
					m_userText = "";
				}
				if (correctKey)
				{
					m_currentResult = core::Scene::SceneResult::GoToLevel;
				}
			}
			else if (core::Game::instance().getKey(8))
			{
				if (m_pressKey != 'd')
				{
					if (m_userText.size() > 0)
					{
						m_userText.erase(std::prev(m_userText.end()));
						m_pressKey = 'd';
					}
				}
			}
			else if (core::Game::instance().getKey('1') && m_userText.size() < 15)
			{
				if (m_pressKey != '1')
				{
					m_pressKey = '1';
					m_userText += "1";
				}
			}
			else if (core::Game::instance().getKey('2') && m_userText.size() < 15)
			{
				if (m_pressKey != '2')
				{
				m_userText += "2";
				m_pressKey = '2';
				}
			}
			else if (core::Game::instance().getKey('3') && m_userText.size() < 15)
			{
				if (m_pressKey != '3')
				{
					m_userText += "3";
					m_pressKey = '3';
				}
			}
			else if (core::Game::instance().getKey('4') && m_userText.size() < 15)
			{
				if (m_pressKey != '4')
				{
					m_userText += "4";
					m_pressKey = '4';
				}
			}
			else if (core::Game::instance().getKey('5') && m_userText.size() < 15)
			{
				if (m_pressKey != '5')
				{
					m_userText += "5";
					m_pressKey = '5';
				}
			}
			else if (core::Game::instance().getKey('6') && m_userText.size() < 15)
			{
				if (m_pressKey != '6')
				{
					m_userText += "6";
					m_pressKey = '6';
				}
			}
			else if (core::Game::instance().getKey('7') && m_userText.size() < 15)
			{
				if (m_pressKey != '7')
				{
					m_userText += "7";
					m_pressKey = '7';
				}
			}
			else if (core::Game::instance().getKey('8') && m_userText.size() < 15)
			{
				if (m_pressKey != '8')
				{
					m_userText += "8";
					m_pressKey = '8';
				}
			}
			else if (core::Game::instance().getKey('9') && m_userText.size() < 15)
			{
				if (m_pressKey != '9')
				{
					m_userText += "9";
					m_pressKey = '9';
				}
			}
			else
			{
				m_pressKey = 'n';
			}
		}

		void PasswordsMenu::render()
		{
			glm::mat4 modelview;
			UpdateButtonMousePositions();
			m_background->render();
			if (m_timeElapsed > 1) 
			{
				m_timeElapsed = 0;
				m_printLimit = !m_printLimit;
			}

			if (m_printLimit)
			{
				m_limit->render();
			}
			m_limit->setPosition(glm::vec2(115 + (FONT_SIZE-5)*m_userText.size(), 164));
			m_text->RenderString(m_userText + "\n", glm::vec2(113,188), glm::vec4(1,1,1,1));
			m_shaderProgram->use();
			m_shaderProgram->setUniformMatrix4f("projection", m_projection);
			m_shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			m_shaderProgram->setUniformMatrix4f("modelview", modelview);
			m_shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
			std::for_each(std::begin(m_buttons), std::end(m_buttons), [](ButtonAction& i_button) {
				i_button.button->Render();
			});
		}

		std::pair<core::Scene::SceneResult, uint32_t> PasswordsMenu::GetSceneResult()
		{
			return{ m_currentResult, m_finalMap };
		}

		void PasswordsMenu::OnMouseButtonReleased(int32_t i_button)
		{
			glm::ivec2 mousePos = core::Game::instance().getMousePos();
			for (const auto& button : m_buttons)
			{
				if (button.button->IsMousePressed(mousePos))
				{
					m_soundSystem.PlayMenuSounds(sound::MenuSounds::ButtonPressed);
					m_currentResult = button.sceneAction;
				}
			}

		}

		void PasswordsMenu::UpdateButtonMousePositions()
		{
			glm::vec2 mousePos = core::Game::instance().getMousePos();
			std::for_each(std::begin(m_buttons), std::end(m_buttons), [mousePos](ButtonAction& i_button) { i_button.button->UpdateMousePosition(mousePos); });
		}

		PasswordsMenu::ButtonAction::ButtonAction(std::unique_ptr<Button> i_button, core::Scene::SceneResult i_sceneAction)
			: button(std::move(i_button))
			, sceneAction(i_sceneAction)
		{

		}

	}
}
