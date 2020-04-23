#include "whitted.h"

#include "../geometry/triangle.h"
#include "../geometry/mesh.h"
#include "../core/uniform_sampler.h"
#include "../math/math.h"
#include "../lights/point_light.h"

glm::vec3 pbr::WhittedIntegrator::Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	Intersection intersection;
	glm::vec3 L = glm::vec3(0.f);

	if (!scene->intersect(ray, intersection))
		return glm::vec3(0.235294, 0.67451, 0.843137);

	const auto triangle = const_cast<Triangle*>(intersection.triangle);
	const auto mesh = std::dynamic_pointer_cast<Mesh, SceneObject>(triangle->scene_object);

	auto ns = intersection.shading.n;
	auto wo = intersection.wo;
	auto o = intersection.point;
	glm::vec3 bias_n = bias * ns;
	bool outside = dot(ray.d, ns) < 0.f;

	for (auto& light : scene->get_lights().get())
	{
		glm::vec3 wi;
		float pdf;
		glm::vec3 Li = light->sample_Li(intersection, sampler->get2D(), &wi, &pdf);
		Intersection empty{};
		float shadow = scene->intersect(Ray(outside ? o + bias_n : o - bias_n, wi), empty);
		auto f = intersection.bsdf->f(wo, wi);
		L += (1.f - shadow) * f * Li * std::abs(dot(wi, ns)) / pdf;
	}

	if (depth + 1 < 10)
	{
		L += reflect(ray, sampler, intersection, depth);
		L += transmit(ray, sampler, intersection, depth);
	}

	return L;
}
