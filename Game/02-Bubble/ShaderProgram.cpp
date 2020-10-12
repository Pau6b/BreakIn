#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"

namespace game
{
ShaderProgram::ShaderProgram()
{
	m_programId = 0;
	m_linked = false;
}


void ShaderProgram::init()
{
	m_programId = glCreateProgram();
}

void ShaderProgram::addShader(const Shader& i_shader)
{
	glAttachShader(m_programId, i_shader.getId());
}

void ShaderProgram::bindFragmentOutput(const std::string& i_outputName)
{
	glBindAttribLocation(m_programId, 0, i_outputName.c_str());
}

GLint ShaderProgram::bindVertexAttribute(const std::string& i_attribName, GLint i_size, GLsizei i_stride, GLvoid* i_firstPointer)
{
	GLint attribPos;

	attribPos = glGetAttribLocation(m_programId, i_attribName.c_str());
	glVertexAttribPointer(attribPos, i_size, GL_FLOAT, GL_FALSE, i_stride, i_firstPointer);

	return attribPos;
}

void ShaderProgram::link()
{
	GLint status;
	char buffer[512];

	glLinkProgram(m_programId);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &status);
	m_linked = (status == GL_TRUE);
	glGetProgramInfoLog(m_programId, 512, NULL, buffer);
	m_errorLog.assign(buffer);
}

void ShaderProgram::free()
{
	glDeleteProgram(m_programId);
}

void ShaderProgram::use()
{
	glUseProgram(m_programId);
}

bool ShaderProgram::isLinked()
{
	return m_linked;
}

const std::string &ShaderProgram::log() const
{
	return m_errorLog;
}

void ShaderProgram::setUniform2f(const std::string& i_uniformName, float i_v0, float i_v1)
{
	GLint location = glGetUniformLocation(m_programId, i_uniformName.c_str());

	if(location != -1)
		glUniform2f(location, i_v0, i_v1);
}

void ShaderProgram::setUniform3f(const std::string& i_uniformName, float i_v0, float i_v1, float i_v2)
{
	GLint location = glGetUniformLocation(m_programId, i_uniformName.c_str());

	if(location != -1)
		glUniform3f(location, i_v0, i_v1, i_v2);
}

void ShaderProgram::setUniform4f(const std::string& i_uniformName, float i_v0, float i_v1, float i_v2, float i_v3)
{
	GLint location = glGetUniformLocation(m_programId, i_uniformName.c_str());

	if(location != -1)
		glUniform4f(location, i_v0, i_v1, i_v2, i_v3);
}

void ShaderProgram::setUniformMatrix4f(const std::string& i_uniformName, glm::mat4& i_mat)
{
	GLint location = glGetUniformLocation(m_programId, i_uniformName.c_str());

	if(location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(i_mat));
}
}