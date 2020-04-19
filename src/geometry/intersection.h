#pragma once

#include <glm/vec2.hpp>

#include "../core/reflection.h"

namespace pbr
{
	class Triangle;

	struct Shading
	{
		glm::vec3 n;
		glm::vec3 dpdu, dpdv;
		glm::vec3 dndu, dndv;
	};

	class Intersection
	{
	public:
		glm::vec2 uv;
		Shading shading;
		const Triangle* triangle{nullptr};
		std::shared_ptr<BSDF> bsdf{nullptr};
		float distance = std::numeric_limits<float>::max();
	};
}
