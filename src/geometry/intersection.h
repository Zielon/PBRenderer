#pragma once

#include <glm/vec2.hpp>

#include "intersectable.h"

namespace pbr
{
	class Intersection
	{
	public:
		glm::vec2 uv;
		const Intersectable* object{nullptr};
		float distance = std::numeric_limits<float>::max();
	};
}
