#include "triangle.h"

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

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
		auto v0 = scene_object->get_vertex(ids.x) * w0;
		auto v1 = scene_object->get_vertex(ids.y) * w1;
		auto v2 = scene_object->get_vertex(ids.z) * w2;

		Shading shading{};
		shading.uv = v0.tex_coords + v1.tex_coords + v2.tex_coords;
		shading.n = normalize(
			transpose(inverse(scene_object->transformation.to_world)) * glm::vec4(
				v0.normal + v1.normal + v2.normal, 1.f));
		shading.dpdu = normalize(cross(shading.n, glm::vec3(0.0f, 1.0f, 0.0f)));
		shading.dpdv = normalize(cross(shading.dpdu, shading.n));

		auto hit = ray.point(t);

		auto radius = length(hit);
		auto theta = glm::atan(glm::sqrt(hit.x * hit.x + hit.y * hit.y), hit.z);
		auto phi = glm::atan(hit.y, hit.x) + (hit.y < 0.0f ? glm::two_pi<float>() : 0.0f);

		if (v0.tex_coords == glm::vec2(0.f) && v1.tex_coords == glm::vec2(0.f) && v2.tex_coords == glm::vec2(0.f))
			shading.uv = glm::vec2(phi * glm::one_over_two_pi<float>(), theta * glm::one_over_pi<float>());

		// TODO FOR NOW OVERRIDE THE DERIVATIVES
		shading.dpdu = glm::vec3(-glm::two_pi<float>() * hit.y, glm::two_pi<float>() * hit.x, 0.0f);
		shading.dpdv =
			glm::vec3(hit.z * glm::cos(phi), hit.z * glm::sin(phi), -radius * glm::sin(theta)) * glm::pi<float>();

		auto w = normalize(shading.n);
		auto u = normalize(cross(shading.dpdu, w));
		auto v = cross(w, u);

		shading.to_local = glm::mat3(u, v, w);
		shading.to_world = transpose(glm::mat3(u, v, w));

		intersection.point = hit;
		intersection.wo = -ray.d;
		intersection.distance = t;
		intersection.triangle = this;
		intersection.shading = shading;

		scene_object->get_material()->compute_BxDF(intersection);

		return true;
	}

	return false;
}

pbr::BBox pbr::Triangle::get_bbox() const{

	return bbox;
}
