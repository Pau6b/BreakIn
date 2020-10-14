#pragma once
#include <vector>

namespace game
{
	template <typename T>
	using Matrix = std::vector<std::vector<T>>;

	template <typename T>
	using Row = std::vector<T>;
}