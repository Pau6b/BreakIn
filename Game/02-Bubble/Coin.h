#pragma once
#include <memory>
#include <map>
#include "ShaderProgram.h"
#include "BreakableBlock.h"

namespace game
{
namespace visuals
{
	class Sprite;
}
}

namespace game
{
namespace gameplay
{

enum class CoinType
{
	Emerald,
	Diamond,
	Gold,
	Iron
};

class Coin : public BreakableBlock
{
public:
	Coin(visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_tileMapDisplay, CoinType i_type );

private:
	static const std::map<CoinType, std::string> s_coinPaths;
};

}
}