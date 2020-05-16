#include "area_light.h"

#include "../geometry/intersection.h"
#include "../geometry/triangle.h"
#include "../geometry/mesh.h"

pbr::AreaLight::~AreaLight(){}

glm::vec3 pbr::AreaLight::sample_Li(
	const Intersection& intersection,
	const std::shared_ptr<Scene>& scene,
	const glm::vec2& u, glm::vec3* wi,
	float* pdf, bool* shadow) const{

	// Sample point on the area light mesh
	auto light = std::dynamic_pointer_cast<Mesh, SceneObject>(scene_light);
	auto sample = light->sample(u);
	auto direction = sample.point - intersection.point;

	*wi = normalize(direction);
	*pdf = 0.f;
	*shadow = true;

	Intersection shadow_test{};
	auto intersects = scene->intersect(Ray(intersection.point + *wi * ray_epsilon, direction), shadow_test);

	if (!intersects || shadow_test.triangle->scene_object->type != LIGHT)
		return glm::vec3(0.f);

	*shadow = false;
	*pdf = 1.f / light->get_area();
	*pdf *= (length(direction) * length(direction)) / std::abs(dot(sample.normal, -*wi));

	if (std::isinf(*pdf)) *pdf = 0.f;

	return L(sample.normal, -*wi);
}

float pbr::AreaLight::pdf_Li(const Intersection& intersection,
                             const std::shared_ptr<Scene>& scene,
                             const glm::vec3& wi) const{

	auto light = std::dynamic_pointer_cast<Mesh, SceneObject>(scene_light);

	Ray ray{intersection.point + wi * ray_epsilon, wi};
	Intersection light_intersection;

	// Only visible samples are considered
	if (!scene->intersect(ray, light_intersection) ||
		light_intersection.triangle->scene_object->type != LIGHT)
		return 0.f;

	auto direction = intersection.point - light_intersection.point;

	// Convert light sample weight to solid angle measure
	auto pdf = (length(direction) * length(direction)) /
		(std::abs(dot(light_intersection.shading.n, -wi)) * light->get_area());

	return std::isinf(pdf) ? 0.f : pdf;
}

float pbr::AreaLight::sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const{

	return 0.f;
}

glm::vec3 pbr::AreaLight::power() const{

	return glm::vec3(0.f);
}

glm::vec3 pbr::AreaLight::L(const glm::vec3& n, const glm::vec3& w) const{

	return two_sided || dot(n, w) > 0 ? luminous : glm::vec3(0.f);
}
