#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Entity.h"

namespace game
{
namespace visuals
{
class ShaderProgram;
class Sprite;
}
}

namespace game
{
namespace gameplay
{
class Portal : Entity
{
public:
	Portal(uint32_t m_currentMine, glm::vec2 i_referenceVector, visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_tileMapDisplay);

	void Update(int i_deltaTime) override;
	void Render() override;
	const glm::vec2 GetReferenceVector() const;
	void SetPosition(const glm::vec2& i_pos);
	const glm::vec2 GetPos() const;

private:
	
	static const std::vector<std::string> m_spritePaths;
	
	std::unique_ptr<visuals::Sprite> m_sprite;
	glm::vec2 m_referenceVector;
	const glm::ivec2& m_tileMapDisplay;
	glm::vec2 m_pos;
};
}
}