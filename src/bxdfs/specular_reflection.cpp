#include "specular_reflection.h"

#include "../math/math.h"

glm::vec3 pbr::SpecularReflection::sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* pdf,
                                            BxDFType* sampledType) const{

	*wi = glm::vec3(-wo.x, -wo.y, wo.z);
	*pdf = 1;
	return fresnel->evaluate(math::cos_theta(*wi)) * r / math::abs_cos_theta(*wi);
}

glm::vec3 pbr::SpecularReflection::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return glm::vec3(0.f);
}

float pbr::SpecularReflection::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
