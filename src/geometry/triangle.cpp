#include "triangle.h"

#include <glm/geometric.hpp>

/**
 * Moller-Trumbore triangle-ray intersection algorithm.
 */
bool pbr::Triangle::intersect(const Ray& ray, Intersection& intersection) const{

	const auto pvec = cross(ray.d, edge1);
	const auto det = dot(edge0, pvec);
	const auto inv_det = 1.0f / det;

	const auto tvec = ray.o - v0;
	const auto w1 = dot(tvec, pvec) * inv_det;

	const auto qvec = cross(tvec, edge0);
	const auto w2 = dot(ray.d, qvec) * inv_det;

	if (w1 < 0.0f || w2 < 0.0f || (w1 + w2) > 1.0f)
	{
		return false;
	}

	const auto t = dot(edge1, qvec) * inv_det;

	const auto w0 = 1.0f - w1 - w2;

	if (t > 0.0f && intersection.distance > t)
	{
		Shading shading{};

		auto v0 = scene_object->get_vertex(ids.x) * w0;
		auto v1 = scene_object->get_vertex(ids.y) * w1;
		auto v2 = scene_object->get_vertex(ids.z) * w2;

		shading.n = normalize(
			transpose(inverse(scene_object->transformation.to_world)) * glm::vec4(
				v0.normal + v1.normal + v2.normal, 1.f));

		intersection.shading = shading;
		intersection.distance = t;
		intersection.triangle = this;
		intersection.uv = v0.tex_coords + v1.tex_coords + v2.tex_coords;

		return true;
	}

	return false;
}

pbr::BBox pbr::Triangle::get_bbox() const{

	return bbox;
}
