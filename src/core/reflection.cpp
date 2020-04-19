#include "reflection.h"

void pbr::BSDF::add(std::shared_ptr<BxDF> bxdf){

	bxdfs.push_back(bxdf);
}

glm::vec3 pbr::BSDF::f(const glm::vec3& wo, const glm::vec3& wi, const glm::vec2& sample, float& pdf, BxDFType type) const{

	return {};
}

float pbr::BSDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
