#include "area_light.h"

glm::vec3 pbr::AreaLight::sample_Li(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi,
                                    float* pdf) const{

	return glm::vec3();
}

float pbr::AreaLight::pdf_Li(const Intersection& ref, const glm::vec3& wi) const{

	return 0.f;
}

float pbr::AreaLight::sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const{

	return 0.f;
}

glm::vec3 pbr::AreaLight::power() const{

	return {};
}