#pragma once
#include <glm/glm.hpp>

namespace game
{
namespace gameplay
{
namespace helpers
{
	glm::vec2 CharToDirVector(char i_char);
	glm::vec2 CalculateExitingPortalDirection(const glm::vec2& i_originalDirection, const glm::vec2& i_enteringDirection, const glm::vec2& i_exitingDirection);
}
}
}