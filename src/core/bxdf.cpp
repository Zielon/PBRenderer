#include "bxdf.h"

#include <glm/gtc/constants.inl>

float pbr::BxDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}

glm::vec3 pbr::LambertianReflection::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return r;
}
