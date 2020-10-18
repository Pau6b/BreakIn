#include "Scene.h"
#include <iostream>

namespace game
{
namespace core
{

void Scene::InitShaders(visuals::ShaderProgram& i_shader, std::string i_vertexFragmentPath, std::string i_fragmentShaderPath)
{
	visuals::Shader vShader, fShader;

	vShader.initFromFile(visuals::ShaderType::VERTEX_SHADER, i_vertexFragmentPath);
	if (!vShader.isCompiled())
	{
		std::cout << "Vertex Shader Error" << "\n";
		std::cout << "" << vShader.log() << "\n\n";
	}
	fShader.initFromFile(visuals::ShaderType::FRAGMENT_SHADER, i_fragmentShaderPath);
	if (!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << "\n";
		std::cout << "" << fShader.log() << "\n\n";
	}
	i_shader.init();
	i_shader.addShader(vShader);
	i_shader.addShader(fShader);
	i_shader.link();
	if (!i_shader.isLinked())
	{
		std::cout << "Shader Linking Error" << "\n";
		std::cout << "" << i_shader.log() << "\n\n";
	}
	i_shader.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

}
}