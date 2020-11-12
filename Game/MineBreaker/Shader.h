#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

namespace game
{
namespace visuals
{
enum class ShaderType { VERTEX_SHADER, FRAGMENT_SHADER };


// This class is able to load to OpenGL a vertex or fragment shader and compile it.
// It can do so from a file or from a string so that shader code can be
// procedurally modified if needed.


class Shader
{

public:
	Shader();

	// These methods should be called with an active OpenGL context
	void initFromSource(const ShaderType i_type, const std::string& i_source);
	bool initFromFile(const ShaderType i_type, const std::string& i_filename);
	void free();

	GLuint getId() const;
	bool isCompiled() const;
	const std::string &log() const;

private:
	bool loadShaderSource(const std::string& i_filename, std::string& i_shaderSource);

private:
	GLuint m_shaderId;
	bool m_compiled;
	std::string m_errorLog;

};
}
}
