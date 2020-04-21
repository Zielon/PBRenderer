#include "point_light.h"

#include <glm/ext/scalar_constants.hpp>
#include "../geometry/intersection.h"

glm::vec3 pbr::PointLight::sample_Li(
	const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const{

	const auto direction = position - intersection.point;
	*wi = normalize(direction);
	*pdf = 1.f;
	return intensity; // / dot(direction, direction);
}

glm::vec3 pbr::PointLight::power() const{

	return 4 * glm::pi<float>() * intensity;
}
