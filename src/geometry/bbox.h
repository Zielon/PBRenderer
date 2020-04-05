#pragma once

#include <limits>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "ray.h"

namespace pbr
{
	class BBox final
	{
	public:
		BBox() = default;

		BBox(const glm::vec3& min, const glm::vec3& max);

		void extend(const glm::vec3& point);

		void extend(const BBox& bbox);

		glm::vec3 diagonal() const;

		int maximum_slab() const;

		float volume() const;

		glm::vec3 centroid() const;

		glm::vec3 min() const;

		glm::vec3 max() const;

		bool intersect(const Ray& ray, float* hit_t0, float* hit_t1) const;

	private:
		glm::vec3 m_min{std::numeric_limits<float>::infinity()};
		glm::vec3 m_max{-std::numeric_limits<float>::infinity()};
	};
}
