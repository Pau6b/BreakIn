#pragma once

namespace game
{
namespace gameplay
{
class Entity
{
public:
	virtual void Update(int i_deltaTime) = 0;
	virtual void Render() = 0;
};
}
}