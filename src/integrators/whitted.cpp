#include "whitted.h"

#include "../geometry/triangle.h"
#include "../geometry/mesh.h"
#include "../core/uniform_sampler.h"
#include "../lights/point_light.h"

glm::vec3 pbr::WhittedIntegrator::Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	Intersection intersection;
	glm::vec3 L = glm::vec3(0.f);

	if (!scene->intersect(ray, intersection))
		return glm::vec3(0.f);

	auto triangle = const_cast<Triangle*>(intersection.triangle);
	auto hit_mesh = dynamic_cast<Mesh*>(triangle->scene_object);

	hit_mesh->get_material()->compute_BxDF(intersection);

	auto ns = intersection.shading.n;
	auto wo = intersection.wo;

	if (hit_mesh->type == LIGHT && hit_mesh->get_area_light())
		L += hit_mesh->get_area_light()->L(ns, wo);

	/*
	 * Calculate Lambertian reflectance for all incoming lights.
	 */
	for (auto& light : scene->get_lights().get())
	{
		glm::vec3 wi{};
		float pdf{0.f};
		bool is_shadow{};
		auto Li = light->sample_Li(intersection, scene, sampler->get2D(), &wi, &pdf, &is_shadow);
		if (Li == glm::vec3(0.f) || pdf == 0.f) continue;
		auto f = intersection.bsdf->f(wo, wi);
		if (f != glm::vec3(0.f) && !is_shadow)
			L += f * Li * std::abs(dot(wi, ns)) / pdf;
	}

	if (depth + 1 < max_depth)
	{
		/*
		 * Whitted's method works only for delta distributions. Meaning a distribution which
		 * is everywhere zero except the reflection or refraction direction. Therefore, for the functions
		 * reflect() and transmit() the sampled BxDF type is selected as TRANSMISSION | REFLECTION | SPECULAR
		 */
		L += reflect(ray, sampler, intersection, depth);
		L += transmit(ray, sampler, intersection, depth);
	}

	// The exiting radiance (measured in W * sr^-1 * m^-2)
	return L;
}
