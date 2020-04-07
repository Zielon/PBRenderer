#pragma once

#include <limits>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "ray.h"

namespace pbr
{
	class BBox
	{
	public:
		BBox() = default;

		BBox(const glm::vec3& min, const glm::vec3& max);

		void extend(const glm::vec3& vec);

		void extend(const glm::vec3& min, const glm::vec3& max);

		void extend(const BBox& bbox);

		glm::vec3 diagonal() const;

		int maximum_extent() const;

		float volume() const;

		float surface_area() const;

		glm::vec3 centroid() const;

		glm::vec3 min() const;

		glm::vec3 max() const;

		bool intersect(const Ray& ray, const glm::vec3& inv_dir) const;

	private:
		glm::vec3 m_min{std::numeric_limits<float>::max()};
		glm::vec3 m_max{std::numeric_limits<float>::lowest()};
	};
}
