#include "triangle.h"

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include "../math/math.h"

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
		// Local space
		auto v0 = scene_object->get_vertex(ids.x) * w0;
		auto v1 = scene_object->get_vertex(ids.y) * w1;
		auto v2 = scene_object->get_vertex(ids.z) * w2;

		Shading shading{};

		auto ns = normalize(v0.normal + v1.normal + v2.normal);
		auto ss = normalize(v0.bitangent + v1.bitangent + v2.bitangent);

		shading.uv = v0.tex_coords + v1.tex_coords + v2.tex_coords;
		shading.n = scene_object->transformation.normal_to_world(ns);
		shading.dpdu = scene_object->transformation.vector_to_local(ss);
		shading.dpdv = scene_object->transformation.vector_to_local(cross(ss, ns));

		// In the case of no tangent and bitangent space, use spherical mapping
		if (v0.tex_coords == glm::vec2(0.f) &&
			v1.tex_coords == glm::vec2(0.f) &&
			v2.tex_coords == glm::vec2(0.f))
		{
			auto hit = scene_object->transformation.vector_to_local(ray.point(t));

			// Spherical coordinates
			auto radius = length(hit);
			auto theta = glm::atan(glm::sqrt(hit.x * hit.x + hit.y * hit.y), hit.z);
			auto phi = glm::atan(hit.y, hit.x) + (hit.y < 0.0f ? glm::two_pi<float>() : 0.0f);

			auto dpdu = glm::vec3(-glm::two_pi<float>() * hit.y, glm::two_pi<float>() * hit.x, 0.0f);
			auto dpdv = glm::vec3(hit.z * glm::cos(phi), hit.z * glm::sin(phi), -radius * glm::sin(theta)) * glm::pi<
				float>();

			shading.uv = glm::vec2(phi * glm::one_over_two_pi<float>(), theta * glm::one_over_pi<float>());
			shading.dpdu = scene_object->transformation.vector_to_local(dpdu);
			shading.dpdv = scene_object->transformation.vector_to_local(dpdv);
		}

		// World space
		intersection.n = scene_object->transformation.normal_to_world(n);
		intersection.point = ray.point(t);
		intersection.wo = -ray.d;
		intersection.distance = t;
		intersection.triangle = this;
		intersection.shading = shading;

		return true;
	}

	return false;
}

pbr::Sample pbr::Triangle::sample(const glm::vec2& u) const{

	float su0 = std::sqrt(u[0]);
	auto b = glm::vec2(1 - su0, u[1] * su0);

	Sample sample{};

	// Local space
	auto v0 = scene_object->get_vertex(ids.x).position * b[0];
	auto v1 = scene_object->get_vertex(ids.y).position * b[1];
	auto v2 = scene_object->get_vertex(ids.z).position * (1 - b[0] - b[1]);

	sample.p = scene_object->transformation.vector_to_world(v0 + v1 + v2);
	sample.n = n;
	sample.pdf = 1.f / area();

	return sample;
}

float pbr::Triangle::area() const{

	return 0.5f * length(cross(edge0, edge1));
}

pbr::BBox pbr::Triangle::get_bbox() const{

	return bbox;
}
