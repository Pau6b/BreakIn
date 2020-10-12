#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


namespace game
{
// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2& i_quadSize, const glm::vec2& i_sizeInSpritesheet, Texture* i_spritesheet, ShaderProgram* i_program);

	Sprite(const glm::vec2& i_quadSize, const glm::vec2& i_sizeInSpritesheet, Texture* i_spritesheet, ShaderProgram* i_program);

	void update(int i_deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int i_nAnimations);
	void setAnimationSpeed(int i_animId, int i_keyframesPerSec);
	void addKeyframe(int i_animId, const glm::vec2& i_frame);
	void changeAnimation(int i_animId);
	int animation() const;
	
	void setPosition(const glm::vec2& i_pos);

private:
	Texture* m_texture;
	ShaderProgram* m_shaderProgram;
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_posLocation, m_texCoordLocation;
	glm::vec2 m_position;
	int m_currentAnimation, m_currentKeyframe;
	float m_timeAnimation;
	glm::vec2 m_texCoordDispl;
	std::vector<AnimKeyframes> m_animations;
};
}

