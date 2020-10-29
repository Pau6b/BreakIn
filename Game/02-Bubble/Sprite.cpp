#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

namespace game
{
namespace visuals
{
Sprite::Sprite(const glm::vec2& i_quadSize, const glm::vec2& i_sizeInSpritesheet, const std::string& i_imagePath, const PixelFormat& i_pixelFormat, ShaderProgram& i_program)
	: m_shaderProgram(i_program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
					  	  i_quadSize.x, 0.f, i_sizeInSpritesheet.x, 0.f, 
						  i_quadSize.x, i_quadSize.y, i_sizeInSpritesheet.x, i_sizeInSpritesheet.y, 
						  0.f, 0.f, 0.f, 0.f, 
						  i_quadSize.x, i_quadSize.y, i_sizeInSpritesheet.x, i_sizeInSpritesheet.y, 
						  0.f, i_quadSize.y, 0.f, i_sizeInSpritesheet.y};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	m_posLocation = m_shaderProgram.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	m_texCoordLocation = m_shaderProgram.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	m_texture.loadFromFile(i_imagePath,i_pixelFormat);
	m_texture.setWrapS(GL_CLAMP_TO_EDGE);
	m_texture.setWrapT(GL_CLAMP_TO_EDGE);
	m_texture.setMinFilter(GL_NEAREST);
	m_texture.setMagFilter(GL_NEAREST);
	m_currentAnimation = -1;
	m_position = glm::vec2(0.f);
}

void Sprite::update(int i_deltaTime)
{
	if(m_currentAnimation >= 0)
	{
		m_timeAnimation += i_deltaTime;
		while(m_timeAnimation > m_animations[m_currentAnimation].millisecsPerKeyframe)
		{
			m_timeAnimation -= m_animations[m_currentAnimation].millisecsPerKeyframe;
			m_currentKeyframe = (m_currentKeyframe + 1) % m_animations[m_currentAnimation].keyframeDispl.size();
		}
		m_texCoordDispl = m_animations[m_currentAnimation].keyframeDispl[m_currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.f));
	m_shaderProgram.setUniformMatrix4f("modelview", modelview);
	m_shaderProgram.setUniform2f("texCoordDispl", m_texCoordDispl.x, m_texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	m_texture.use();
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(m_posLocation);
	glEnableVertexAttribArray(m_texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &m_vbo);
}

void Sprite::setNumberAnimations(int i_nAnimations)
{
	m_animations.clear();
	m_animations.resize(i_nAnimations);
}

void Sprite::setAnimationSpeed(int i_animId, int i_keyframesPerSec)
{
	if(i_animId < int(m_animations.size()))
		m_animations[i_animId].millisecsPerKeyframe = 1000.f / i_keyframesPerSec;
}

void Sprite::addKeyframe(int i_animId, const glm::vec2& i_displacement)
{
	if(i_animId < int(m_animations.size()))
		m_animations[i_animId].keyframeDispl.push_back(i_displacement);
}

void Sprite::changeAnimation(int i_animId)
{
	if(i_animId < int(m_animations.size()))
	{
		m_currentAnimation = i_animId;
		m_currentKeyframe = 0;
		m_timeAnimation = 0.f;
		m_texCoordDispl = m_animations[i_animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return m_currentAnimation;
}

void Sprite::setPosition(const glm::vec2& i_pos)
{
	m_position = i_pos;
}

}
}
