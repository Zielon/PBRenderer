#include "triangle.h"

#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include "../math/math.h"

// http://www.pbr-book.org/3ed-2018/Shapes/Managing_Rounding_Error.html
inline float gamma(int n){

	return (n * std::numeric_limits<float>::epsilon() * 0.5) / (1 - n * std::numeric_limits<float>::epsilon() * 0.5);
}

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
		// World space
		auto v0 = scene_object->get_vertex(ids.x) * w0;
		auto v1 = scene_object->get_vertex(ids.y) * w1;
		auto v2 = scene_object->get_vertex(ids.z) * w2;

		Shading shading{};

		auto ns = normalize(v0.normal + v1.normal + v2.normal);
		auto ss = normalize(v0.bitangent + v1.bitangent + v2.bitangent);
		auto tt = normalize(v0.tangent + v1.tangent + v2.tangent);

		shading.uv = v0.tex_coords + v1.tex_coords + v2.tex_coords;
		shading.n = ns;
		shading.dpdu = scene_object->transformation.vector_to_local(ss);
		shading.dpdv = scene_object->transformation.vector_to_local(tt);

		// In the case of neither tangent nor bitangent space available, use spherical mapping
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

		auto sum = abs(v0.position) + abs(v1.position) + abs(v2.position);

		// World space
		intersection.error = gamma(7) * glm::vec3(sum.x, sum.y, sum.z);
		intersection.n = n;
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

	// World space
	auto v0 = scene_object->get_vertex(ids.x).position * b[0];
	auto v1 = scene_object->get_vertex(ids.y).position * b[1];
	auto v2 = scene_object->get_vertex(ids.z).position * (1 - b[0] - b[1]);
	auto sum = abs(v0) + abs(v1) + abs(v2);

	sample.error = gamma(7) * glm::vec3(sum.x, sum.y, sum.z);
	sample.point = v0 + v1 + v2;
	sample.normal = n;
	sample.pdf = 1.f / area;

	return sample;
}

pbr::BBox pbr::Triangle::get_bbox() const{

	return bbox;
}
