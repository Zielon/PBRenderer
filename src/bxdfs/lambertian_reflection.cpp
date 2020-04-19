#include "lambertian_reflection.h"

#include <glm/gtc/constants.inl>

glm::vec3 pbr::LambertianReflection::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return r * glm::one_over_pi<float>();
}
