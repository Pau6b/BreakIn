#pragma once
#include <string>
#include <GL/glew.h>


namespace game
{
namespace visuals
{
enum class PixelFormat {TEXTURE_PIXEL_FORMAT_RGB, TEXTURE_PIXEL_FORMAT_RGBA};


// The texture class loads images an passes them to OpenGL
// storing the returned id so that it may be applied to any drawn primitives


class Texture
{

public:
	Texture();

	bool loadFromFile(const std::string& i_filename, PixelFormat i_format);
	void loadFromGlyphBuffer(unsigned char* i_buffer, int i_width, int i_height);

	void createEmptyTexture(int i_width, int i_height);
	void loadSubtextureFromGlyphBuffer(unsigned char* i_buffer, int i_x, int i_y, int i_width, int i_height);
	void generateMipmap();
	
	void setWrapS(GLint i_value);
	void setWrapT(GLint i_value);
	void setMinFilter(GLint i_value);
	void setMagFilter(GLint i_value);
	
	void use() const;
	
	int width() const { return m_widthTex; }
	int height() const { return m_heightTex; }

private:
	int m_widthTex, m_heightTex;
	GLuint m_texId;
	GLint m_wrapS, m_wrapT, m_minFilter, m_magFilter;

};
}
}