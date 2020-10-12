#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Shader.h"


// Using the Shader class ShaderProgram can link a vertex and a fragment shader
// together, bind input attributes to their corresponding vertex shader names, 
// and bind the fragment output to a name from the fragment shader

namespace game
{
class ShaderProgram
{

public:
	ShaderProgram();

	void init();
	void addShader(const Shader& i_shader);
	void bindFragmentOutput(const std::string& i_outputName);
	GLint bindVertexAttribute(const std::string& i_attribName, GLint i_size, GLsizei i_stride, GLvoid* i_firstPointer);
	void link();
	void free();

	void use();

	// Pass uniforms to the associated shaders
	void setUniform2f(const std::string& i_uniformName, float i_v0, float i_v1);
	void setUniform3f(const std::string& i_uniformName, float i_v0, float i_v1, float i_v2);
	void setUniform4f(const std::string& i_uniformName, float i_v0, float i_v1, float i_v2, float i_v3);
	void setUniformMatrix4f(const std::string& i_uniformName, glm::mat4& i_mat);

	bool isLinked();
	const std::string &log() const;

private:
	GLuint m_programId;
	bool m_linked;
	std::string m_errorLog;

};
}
