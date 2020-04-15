#pragma once

#include <glm/vec2.hpp>

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
		glm::vec3 barycentric;
		Shading shading;
		const Triangle* triangle{nullptr};
		float distance = std::numeric_limits<float>::max();
	};
}
