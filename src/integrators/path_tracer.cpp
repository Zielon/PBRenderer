#include "path_tracer.h"

#include <glm/gtx/component_wise.hpp>
#include "../geometry/mesh.h"
#include "../math/math.h"

inline float power_heuristic(int nf, float fPdf, int ng, float gPdf){

	auto f = nf * fPdf;
	auto g = ng * gPdf;

	return (f * f) / (f * f + g * g);
}

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

		// Sample illumination from lights to find path contribution.
		// (But skip this for perfectly specular BSDFs.)
		if (intersection.bsdf->num_components(BxDFType(ALL & ~SPECULAR)) > 0)
		{
			// Randomly select light
			auto n_lights = int(scene->get_lights().get().size());
			auto selected = std::min(int(sampler->get1D() * n_lights), n_lights - 1);
			auto pdf = 1.f / n_lights;
			auto& light = scene->get_lights().get()[selected];

			glm::vec3 Ld(0.f);

			/* Estimate directly */
			{
				// Sample light source with multiple importance sampling
				auto bsdf_flags = BxDFType(ALL & ~SPECULAR);
				glm::vec3 wi{0.f};
				float light_pdf{0.f};
				float scattering_pdf{0.f};
				bool is_shadow{};
				auto Li = light->sample_Li(intersection, scene, sampler->get2D(), &wi, &light_pdf, &is_shadow);

				if (light_pdf > 0.f && Li != glm::vec3(0.f))
				{
					auto f = intersection.bsdf->f(wo, wi, bsdf_flags) * std::abs(dot(wi, ns));
					scattering_pdf = intersection.bsdf->pdf(wo, wi, bsdf_flags);

					if (f != glm::vec3(0.f))
					{
						if (is_shadow) Li = glm::vec3(0.f);

						// Add light's contribution to reflected radiance
						if (Li != glm::vec3(0.f))
						{
							float weight = power_heuristic(1, light_pdf, 1, scattering_pdf);
							Ld += f * Li * weight / light_pdf;
						}
					}
				}

				// Sample scattered direction for surface interactions
				BxDFType type;
				auto f = intersection.bsdf->sample_f(wo, &wi, sampler, &scattering_pdf, bsdf_flags, &type);
				f *= std::abs(dot(wi, ns));
				auto is_specular = (type & SPECULAR) != 0;

				if (scattering_pdf > 0.f && f != glm::vec3(0.f))
				{
					// Account for light contributions along sampled direction wi
					float weight = 1.f;
					if (!is_specular)
					{
						light_pdf = light->pdf_Li(intersection, scene, wi);
						if (light_pdf == 0) return Ld;
						weight = power_heuristic(1, scattering_pdf, 1, light_pdf);
					}

					// Find intersection
					Intersection light_intersection;
					Ray surface_ray = {o + wi * ray_epsilon, wi};
					auto intersects = scene->intersect(surface_ray, light_intersection);

					Li = glm::vec3(0.f);

					if (intersects)
					{
						auto area_light = mesh->get_area_light();
						if (mesh->type == LIGHT && area_light == light)
							Li = area_light->L(light_intersection.shading.n, -wi);

						if (Li != glm::vec3(0.f))
							Ld += f * Li * weight / scattering_pdf;
					}
				}
			} /* Estimate directly */

			L += beta * (Ld / pdf);
		}

		// Sample BSDF to get new path direction
		float pdf{0.f};
		glm::vec3 wi{0.f};
		BxDFType flags;

		auto f = intersection.bsdf->sample_f(wo, &wi, sampler, &pdf, BxDFType(ALL), &flags);
		beta *= f * std::abs(dot(wi, ns)) / pdf;

		//ray = {math::offset_ray_origin(o, ns, intersection.error, wi), wi};
		ray = {o + wi * ray_epsilon, wi};

		is_specular_ray = (flags & SPECULAR) != 0;

		// Russian roulette
		if (compMax(beta) < 1.f && depth > 3)
		{
			float q = std::max(float(.05), 1.f - compMax(beta));
			if (sampler->get1D() < q) break;
			beta /= 1 - q;
		}

		depth++;
	}

	return L;
}
