#include "area_light.h"

#include <glm/ext/scalar_constants.hpp>

#include "../geometry/intersection.h"
#include "../geometry/triangle.h"
#include "../geometry/mesh.h"

glm::vec3 pbr::AreaLight::sample_Li(
	const Intersection& intersection, const std::shared_ptr<Scene>& scene, const glm::vec2& u, glm::vec3* wi,
	float* pdf, bool* shadow) const{

	auto mesh = std::dynamic_pointer_cast<Mesh, SceneObject>(scene_object);
	auto sample = mesh->sample(u);
	auto direction = sample.p - intersection.point;

	Intersection light{};
	*wi = normalize(direction);
	*shadow = !scene->intersect(Ray(intersection.point + *wi * 0.001f, *wi), light);
	*pdf = 0.f;

	if (!*shadow && light.triangle->scene_object->type != LIGHT)
		return glm::vec3(0.f);

	*pdf = sample.pdf;
	*pdf *= length(direction) * length(direction) / std::abs(dot(sample.n, *wi));

	if (std::isinf(*pdf)) *pdf = 0.f;

	return L(sample.n, -*wi);
}

float pbr::AreaLight::pdf_Li(const Intersection& intersection, const glm::vec3& wi) const{

	auto* triangle = const_cast<Triangle*>(intersection.triangle);

	return 1.f / triangle->area();
}

float pbr::AreaLight::sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const{

	auto mesh = std::dynamic_pointer_cast<Mesh, SceneObject>(scene_object);
	auto sample = mesh->sample(u);
	auto direction = sample.p - intersection.point;

	return 0.f;
}

glm::vec3 pbr::AreaLight::power() const{

	return 2.f * luminous * scene_object->get_bbox().surface_area() / 2.f * glm::pi<float>();
}

glm::vec3 pbr::AreaLight::L(const glm::vec3& n, const glm::vec3& w) const{

	return two_sided || dot(n, w) > 0 ? luminous : glm::vec3(0.f);
}
