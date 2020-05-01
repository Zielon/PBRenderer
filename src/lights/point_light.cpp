#include "point_light.h"

#include <glm/ext/scalar_constants.hpp>
#include "../geometry/intersection.h"
#include "../core/scene.h"

glm::vec3 pbr::PointLight::sample_Li(
	const Intersection& intersection, const std::shared_ptr<Scene>& scene, const glm::vec2& u, glm::vec3* wi,
	float* pdf, bool* shadow) const{

	Intersection empty{};

	const auto direction = position - intersection.point;

	*wi = normalize(direction);
	*pdf = 1.f;
	*shadow = scene->intersect(Ray(intersection.point, *wi), empty);

	return intensity / length(direction) * length(direction);
}

float pbr::PointLight::pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene, const glm::vec3& wi) const{

	return 0.f;
}

float pbr::PointLight::sample_Le(
	const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const{

	return 0.f;
}

glm::vec3 pbr::PointLight::power() const{

	return 4 * glm::pi<float>() * intensity;
}
