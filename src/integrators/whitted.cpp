#include "whitted.h"

#include <chrono>
#include <thread>

#include "../geometry/triangle.h"
#include "../geometry/mesh.h"
#include "../core/uniform_sampler.h"
#include "../math/math.h"
#include "../lights/point_light.h"

glm::vec3 pbr::WhittedIntegrator::Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	float bias = 0.00001f;
	Intersection intersection;
	glm::vec3 L = glm::vec3(0.f);

	if (!scene->intersect(ray, intersection)) return L;

	const auto triangle = const_cast<Triangle*>(intersection.triangle);
	const auto mesh = std::dynamic_pointer_cast<Mesh, SceneObject>(triangle->scene_object);

	auto n = intersection.shading.n;
	auto wo = intersection.wo;
	glm::vec3 o = dot(ray.d, n) < 0 ? intersection.point + n * bias : intersection.point - n * bias;

	for (auto& light : scene->get_lights().get())
	{
		glm::vec3 wi;
		float pdf = 1.f;
		glm::vec3 Li = light->sample_Li(intersection, sampler->get2D(), &wi, &pdf);
		Intersection insect;
		auto shadow_ray = Ray(o, wi);
		float shadow = scene->intersect(shadow_ray, insect);
		auto f = intersection.bsdf->f(wo, wi);
		L += (1.f - shadow) * f * Li * std::max(0.f, dot(wi, n)) / pdf;
	}

	if (depth + 1 < 8)
	{
		L += reflect(ray, sampler, intersection, depth);
		//L += transmit(ray, sampler, intersection, depth);
	}

	return L;
}
