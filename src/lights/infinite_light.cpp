#include "infinite_light.h"

glm::vec3 pbr::InfiniteLight::power() const{

	return {};
}

glm::vec3 pbr::InfiniteLight::Le(const Intersection& intersection) const{
	
	return {};
}

glm::vec3 pbr::InfiniteLight::sample_Li(const Intersection& intersection, const std::shared_ptr<Scene>& scene,
                                        const glm::vec2& u, glm::vec3* wi, float* pdf, bool* shadow) const{

	return {};
}

float pbr::InfiniteLight::pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene,
                                 const glm::vec3& wi) const{

	return 0.f;
}

float pbr::InfiniteLight::sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi,
                                    float* pdf) const{

	return 0.f;
}
