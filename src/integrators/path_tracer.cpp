#include "path_tracer.h"

#include <glm/gtx/component_wise.hpp>
#include "../geometry/mesh.h"
#include "../math/math.h"

glm::vec3 pbr::PathTracer::Li(const Ray& camera_ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	Ray ray{camera_ray.o, camera_ray.d};
	glm::vec3 L{0.f};
	glm::vec3 beta{1.f};
	auto is_specular_ray = false;

	while (true)
	{
		Intersection intersection;

		if (!scene->intersect(ray, intersection) || depth > max_depth)
			break;

		auto triangle = const_cast<Triangle*>(intersection.triangle);
		auto mesh = std::dynamic_pointer_cast<Mesh, SceneObject>(triangle->scene_object);

		mesh->get_material()->compute_BxDF(intersection);

		auto wo = intersection.wo;
		auto o = intersection.point;
		auto ns = intersection.shading.n;

		if (mesh->type == LIGHT && mesh->get_area_light())
			L += beta * mesh->get_area_light()->L(ns, wo);

		// Direct illumination estimation
		if (intersection.bsdf->num_components(BxDFType(ALL & ~SPECULAR)) > 0)
		{
			float light_pdf;
			auto light = select_light(sampler->get1D(), &light_pdf);
			auto Ld = direct_illumination(intersection, light, sampler);
			L += beta * (Ld / light_pdf);
		}

		// Sample BSDF to get new path direction
		float pdf{0.f};
		glm::vec3 wi{0.f};
		BxDFType flags;

		auto f = intersection.bsdf->sample_f(wo, &wi, sampler, &pdf, BxDFType(ALL), &flags);

		if (f == glm::vec3(0.f) || pdf == 0.f) break;

		beta *= f * std::abs(dot(wi, ns)) / pdf;
		is_specular_ray = (flags & SPECULAR) != 0;

		// Russian roulette
		if (compMax(beta) < 1.f && depth > 3)
		{
			float q = std::max(float(.05), 1.f - compMax(beta));
			if (sampler->get1D() < q) break;
			beta /= 1 - q;
		}

		ray = {o + wi * ray_epsilon, wi};
		depth++;
	}

	return L;
}
