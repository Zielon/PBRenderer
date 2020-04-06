#pragma once

#include "intersectable.h"

namespace pbr
{
	class Triangle : public Intersectable
	{
	public:
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n, glm::vec3 min,
		         glm::vec3 max) : bbox(BBox(min, max)), v0(v0), v1(v1), v2(v2), n(n){ };

		bool intersect(const Ray& ray, Intersection& intersection) override;

		BBox getBBox() override;

		BBox bbox;
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 n;
	};
}
