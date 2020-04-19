#pragma once

#include "../geometry/intersection.h"

namespace pbr
{
	template <typename T>
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual T evaluate(Intersection& intersection) const = 0;
	};
}
