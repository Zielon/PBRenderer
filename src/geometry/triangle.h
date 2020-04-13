#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "bbox.h"
#include "intersection.h"
#include "intersectable.h"
#include <memory>
#include "../core/scene.h"

namespace pbr
{
	class Triangle : Intersectable
	{
	public:
		Triangle(glm::vec3 ids, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 min,
		         glm::vec3 max, SceneObject* object) : edge0(v1 - v0), edge1(v2 - v0), v0(v0), v1(v1), v2(v2),
		                                               n(normalize(cross(v1 - v0, v2 - v0))), object(object), ids(ids),
		                                               bbox(BBox(min, max)){ };

		bool intersect(const Ray& ray, Intersection& intersection) const;

		BBox get_bbox() const;

		glm::vec3 edge0;
		glm::vec3 edge1;
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 n;

		const SceneObject* object{nullptr};

		glm::ivec3 ids;

	private:
		BBox bbox;
	};
}
