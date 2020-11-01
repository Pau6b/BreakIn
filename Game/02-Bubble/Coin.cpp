#include "Coin.h"
#include "Sprite.h"
#include "Texture.h"

namespace game
{
namespace gameplay
{
const std::map<CoinType, std::string> Coin::s_coinPaths = { {CoinType::Diamond, "images/diamond.png"},
															{CoinType::Emerald, "images/emerald.png"},
															{CoinType::Iron, "images/iron.png"},
															{CoinType::Gold, "images/gold.png"},
};

Coin::Coin(visuals::ShaderProgram& i_shaderProgram, const glm::ivec2& i_tileMapDisplay, CoinType i_type)
	: BreakableBlock(1,std::make_unique<visuals::Sprite>(glm::vec2(32, 32), glm::vec2(1, 1), s_coinPaths.at(i_type), visuals::PixelFormat::TEXTURE_PIXEL_FORMAT_RGB, i_shaderProgram), BreakableBlock::BreakableBlockAnimationSize::Big, i_tileMapDisplay, i_shaderProgram)
{

}

}
}