#include "reflection.h"
#include "bxdf.h"

pbr::BSDF::BSDF(Intersection& intersection){ }

void pbr::BSDF::add(std::shared_ptr<BxDF> bxdf){

	bxdfs.push_back(bxdf);
}

glm::vec3 pbr::BSDF::f(const glm::vec3& woW, const glm::vec3& wiW, BxDFType flags) const{

	return bxdfs[0]->f(woW, wiW);
}

glm::vec3 pbr::BSDF::sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& u, float* pdf, BxDFType type,
                              BxDFType* sampledType) const{

	return {};
}

float pbr::BSDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
