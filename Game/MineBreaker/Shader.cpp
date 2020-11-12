#include <fstream>
#include "Shader.h"


namespace game
{
namespace visuals
{
Shader::Shader()
{
	m_shaderId = 0;
	m_compiled = false;
}


void Shader::initFromSource(const ShaderType i_type, const std::string& i_source)
{
	const char *sourcePtr = i_source.c_str();
	GLint status;
	char buffer[512];

	switch (i_type)
	{
	case ShaderType::VERTEX_SHADER:
		m_shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::FRAGMENT_SHADER:
		m_shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}
	if (m_shaderId == 0)
		return;
	glShaderSource(m_shaderId, 1, &sourcePtr, NULL);
	glCompileShader(m_shaderId);
	glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &status);
	m_compiled = (status == GL_TRUE);
	glGetShaderInfoLog(m_shaderId, 512, NULL, buffer);
	m_errorLog.assign(buffer);
}

bool Shader::initFromFile(const ShaderType i_type, const std::string& i_filename)
{
	std::string shaderSource;

	if (!loadShaderSource(i_filename, shaderSource))
		return false;
	initFromSource(i_type, shaderSource);

	return true;
}

void Shader::free()
{
	glDeleteShader(m_shaderId);
	m_shaderId = 0;
	m_compiled = false;
}

GLuint Shader::getId() const
{
	return m_shaderId;
}

bool Shader::isCompiled() const
{
	return m_compiled;
}

const std::string &Shader::log() const
{
	return m_errorLog;
}

bool Shader::loadShaderSource(const std::string& i_filename, std::string& i_shaderSource)
{
	std::ifstream fin;

	fin.open(i_filename.c_str());
	if (!fin.is_open())
		return false;
	i_shaderSource.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

	return true;
}
}
}