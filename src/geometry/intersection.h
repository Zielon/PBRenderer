#pragma once

#include <glm/vec2.hpp>

#include "../core/reflection.h"

namespace pbr
{
	class Triangle;

	struct Shading
	{
		glm::vec3 n;
		glm::vec2 uv;
		glm::vec3 dpdu;
		glm::vec3 dpdv;
		glm::mat3 to_local;
		glm::mat3 to_world;
	};

	class Intersection
	{
	public:
		glm::vec3 point;
		glm::vec3 wo;
		Shading shading;
		const Triangle* triangle{nullptr};
		std::shared_ptr<BSDF> bsdf{nullptr};
		float distance = std::numeric_limits<float>::max();
	};
}
