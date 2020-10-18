#pragma once
#include <stdint.h>
#include <utility>
#include "ShaderProgram.h"

namespace game
{
namespace core
{

class Scene
{
public:
	enum class SceneResult
	{
		NotFinished,
		GoToLevel,
		GoToMainMenu
	};

	virtual void init() = 0;
	virtual void update(int i_deltaTime) = 0;
	virtual void render() = 0;
	virtual std::pair<SceneResult, uint32_t> GetSceneResult() = 0;
	virtual void OnMouseButtonReleased(int32_t i_button) {};
protected:
	static void InitShaders(visuals::ShaderProgram& i_shader, std::string i_vertexFragmentPath, std::string i_fragmentShaderPath);
};

}
}