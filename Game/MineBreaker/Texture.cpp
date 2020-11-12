#include <SOIL.h>
#include "Texture.h"

namespace game
{
namespace visuals
{
Texture::Texture()
{
	m_wrapS = GL_REPEAT;
	m_wrapT = GL_REPEAT;
	m_minFilter = GL_LINEAR_MIPMAP_LINEAR;
	m_magFilter = GL_LINEAR_MIPMAP_LINEAR;
}


bool Texture::loadFromFile(const std::string& i_filename, PixelFormat i_format)
{
	unsigned char *image = NULL;
	
	switch(i_format)
	{
	case PixelFormat::TEXTURE_PIXEL_FORMAT_RGB:
		image = SOIL_load_image(i_filename.c_str(), &m_widthTex, &m_heightTex, 0, SOIL_LOAD_RGB);
		break;
	case PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA:
		image = SOIL_load_image(i_filename.c_str(), &m_widthTex, &m_heightTex, 0, SOIL_LOAD_RGBA);
		break;
	}
	if(image == NULL)
		return false;
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	switch(i_format)
	{
	case PixelFormat::TEXTURE_PIXEL_FORMAT_RGB:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_widthTex, m_heightTex, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		break;
	case PixelFormat::TEXTURE_PIXEL_FORMAT_RGBA:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_widthTex, m_heightTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		break;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	
	return true;
}

void Texture::loadFromGlyphBuffer(unsigned char* i_buffer, int i_width, int i_height)
{
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, i_width, i_height, 0, GL_RED, GL_UNSIGNED_BYTE, i_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::createEmptyTexture(int i_width, int i_height)
{
	glGenTextures(1, &m_texId);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, i_width, i_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::loadSubtextureFromGlyphBuffer(unsigned char* i_buffer, int i_x, int i_y, int i_width, int i_height)
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, i_x, i_y, i_width, i_height, GL_RED, GL_UNSIGNED_BYTE, i_buffer);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::generateMipmap()
{
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenerateMipmap(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Texture::setWrapS(GLint i_value)
{
	m_wrapS = i_value;
}

void Texture::setWrapT(GLint i_value)
{
	m_wrapT = i_value;
}

void Texture::setMinFilter(GLint i_value)
{
	m_minFilter = i_value;
}

void Texture::setMagFilter(GLint i_value)
{
	m_magFilter = i_value;
}

void Texture::use() const
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFilter);
}

}
}