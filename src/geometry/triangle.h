#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "bbox.h"
#include "intersection.h"
#include "../core/scene.h"
#include "../lights/area_light.h"

namespace pbr
{
	struct Sample
	{
		glm::vec3 point;
		glm::vec3 normal;
		glm::vec3 error;
		float pdf;
	};

	/**
	 * Triangle is defined in the world space
	 */
	class Triangle
	{
	public:
		Triangle(glm::vec3 ids, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n, glm::vec3 min,
		         glm::vec3 max, SceneObject* object) : v0(v0), v1(v1), v2(v2), n(n),
		                                               ids(ids), edge0(v1 - v0), edge1(v2 - v0),
		                                               scene_object(object), bbox(BBox(min, max)){
			area = 0.5f * length(cross(edge0, edge1));
		};

		bool intersect(const Ray& ray, Intersection& intersection) const;

		Sample sample(const glm::vec2& u) const;

		BBox get_bbox() const;

		float area;
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 n;
		glm::ivec3 ids;
		glm::vec3 edge0;
		glm::vec3 edge1;

		const std::shared_ptr<SceneObject> scene_object{nullptr};

	private:
		BBox bbox;
	};
}
