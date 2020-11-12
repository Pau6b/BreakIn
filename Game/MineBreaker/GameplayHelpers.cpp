#include "GameplayHelpers.h"
#include "Log.h"

namespace game
{
namespace gameplay
{
namespace helpers
{

	glm::vec2 CharToDirVector(char i_char)
	{
		if (i_char == 'R')
		{
			return glm::vec2(1, 0);
		}
		else if (i_char == 'L')
		{
			return glm::vec2(-1, 0);
		}
		else if (i_char == 'U')
		{
			return glm::vec2(0, -1);
		}
		else if (i_char == 'D')
		{
			return glm::vec2(0, 1);
		}
		BreakIf(true, "Char to dir, wrong char");
		return glm::vec2(0, 0);
	}

	glm::vec2 CalculateExitingPortalDirection(const glm::vec2& i_originalDirection, const glm::vec2& i_enteringDirection, const glm::vec2& i_exitingDirection)
	{
		if (i_enteringDirection == i_exitingDirection)
		{
			return i_originalDirection;
		}
		else if (i_enteringDirection == -i_exitingDirection)
		{
			return -i_originalDirection;
		}
		//No trivial case here, we calculate angle between entering and exitingVector
		float enterExitDot = glm::dot(i_enteringDirection, i_exitingDirection);
		float angle = std::acos(enterExitDot / (glm::length(i_enteringDirection) * glm::length(i_exitingDirection)));
		glm::vec2 result = i_originalDirection;
		result.x = i_originalDirection.x * std::cos(angle) - i_originalDirection.y * std::sin(angle);
		result.y = i_originalDirection.x * std::sin(angle) + i_originalDirection.y * std::cos(angle);
		return result;
	}

}
}
}