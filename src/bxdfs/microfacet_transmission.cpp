#include "microfacet_transmission.h"

glm::vec3 pbr::MicrofacetTransmission::f(const glm::vec3& wo, const glm::vec3& wi) const{

	return {};}

glm::vec3 pbr::MicrofacetTransmission::sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& sample, float* pdf,
                                                BxDFType* sampledType) const{

	return {};
}

float pbr::MicrofacetTransmission::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0;
}
