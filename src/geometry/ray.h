#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace pbr
{
	class Ray
	{
	public:
		Ray() = default;

		Ray(glm::vec3 origin, glm::vec3 direction): o(origin), d(normalize(direction)){}

		Ray spawn(const glm::vec3& dir, glm::vec3 orig, const glm::vec3& n) const;

		glm::vec3 point(float t) const{

			return o + d * t;
		}

		glm::vec3 o;
		glm::vec3 d;
	};
}
