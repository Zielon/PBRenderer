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
		intersection.distance = t;
		intersection.triangle = this;
		intersection.uv = v0 * w0 + v1 * w1 + v2 * w2;
		intersection.barycentric = glm::vec3(w0, w1, w2);

		Shading shading{};

		auto v0 = scene_object->get_vertex(ids.x).normal * w0;
		auto v1 = scene_object->get_vertex(ids.y).normal * w1;
		auto v2 = scene_object->get_vertex(ids.z).normal * w2;

		shading.n = normalize(transpose(inverse(scene_object->transformation.to_world)) * glm::vec4(v0 + v1 + v2, 1.f));

		intersection.shading = shading;

		return true;
	}

	return false;
}

pbr::BBox pbr::Triangle::get_bbox() const{

	return bbox;
}
