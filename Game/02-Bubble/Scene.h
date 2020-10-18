#pragma once

namespace game
{
namespace core
{

class Scene
{
public:
	virtual void init() = 0;
	virtual void update(int i_deltaTime) = 0;
	virtual void render() = 0;
};

}
}