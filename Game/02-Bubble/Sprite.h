#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <functional>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"


namespace game
{
namespace visuals
{
// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class Sprite
{

public:
	enum class AnimationAdvanceType
	{
		Free,
		NumberOfFrames
	};

	// Textured quads can only be created inside an context
	Sprite(const glm::vec2& i_quadSize, const glm::vec2& i_sizeInSpritesheet, const std::string& i_imagePath, const PixelFormat& i_pixelFormat, ShaderProgram& i_program);

	void update(int i_deltaTime);
	void render() const;
	void free();

	void SetAnimationAdvanceType(AnimationAdvanceType i_animationAdvanceType);
	void SetFramesToAdvance(uint32_t i_numberOfFramesToAdvance);
	void SetOnFramesAdvancedCallback(std::function<void(uint32_t i_numberOfAdvancedFrames)> i_onAnimationFinishedCallback);
	void setNumberAnimations(int i_nAnimations);
	void setAnimationSpeed(int i_animId, int i_keyframesPerSec);
	void addKeyframe(int i_animId, const glm::vec2& i_frame);
	void setCurrentKeyFrame(uint32_t i_keyFrame);
	void changeAnimation(int i_animId);
	int animation() const;
	
	void setPosition(const glm::vec2& i_pos);

private:
	Texture m_texture;
	ShaderProgram& m_shaderProgram;
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_posLocation, m_texCoordLocation;
	glm::ivec2 m_position;
	int m_currentAnimation, m_currentKeyframe;
	float m_timeAnimation;
	glm::vec2 m_texCoordDispl;
	std::vector<AnimKeyframes> m_animations;
	std::vector<std::function<void(uint32_t i_numberOfAdvancedFrames)>> m_onFramesAdvancedCallback;
	AnimationAdvanceType m_animationAdvanceType = AnimationAdvanceType::Free;
	uint32_t m_framesToAdvance = 0;
	uint32_t m_totalNumberOfFramesAdvanced = 0;
};
}
}