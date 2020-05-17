#include "path_tracer.h"

#include <glm/gtx/component_wise.hpp>
#include "../geometry/mesh.h"
#include "../math/math.h"

glm::vec3 pbr::PathTracer::Li(const Ray& camera_ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	Ray ray{camera_ray.o, camera_ray.d};
	glm::vec3 L{0.f};
	glm::vec3 beta{1.f};
	auto is_specular_ray = false;

	while (depth <= max_depth)
	{
		Intersection intersection;
		Triangle* triangle{};
		Mesh* hit_mesh{};
		bool found = scene->intersect(ray, intersection);

		if (found)
		{
			triangle = const_cast<Triangle*>(intersection.triangle);
			hit_mesh = dynamic_cast<Mesh*>(triangle->scene_object);
		}

		/*
		 * There are two exceptions: the first is at the initial intersection point of camera rays,
		 * since this is the only opportunity to include emission from directly visible objects.
		 * The second is when the sampled direction from the last path vertex was from a specular BSDF component:
		 * in this case, the previous iteration’s direct illumination estimate
		 * could not evaluate the associated integrand containing a Dirac delta function, and we must account for it here.
		 * @ref pbr-book
		 */
		if (depth == 0 || is_specular_ray)
		{
			if (found)
			{
				if (hit_mesh->type == LIGHT && hit_mesh->get_area_light()) {
					L += beta * hit_mesh->get_area_light()->L(intersection.shading.n, intersection.wo);
					break;
				}
			}
			else
			{
				for (const auto& light : scene->get_environment_lights().get())
					L += beta * light->Le(ray);
			}
		}

		if (!found) break;

		hit_mesh->get_material()->compute_BxDF(intersection);

		auto wo = intersection.wo;
		auto o = intersection.point;
		auto ns = intersection.shading.n;

		/*
		 * Direct illumination estimation
		 * Sample illumination from lights to find path contribution while skipping perfectly specular
		 */
		if (intersection.bsdf->num_components(BxDFType(ALL & ~SPECULAR)) > 0)
		{
			if (!scene->get_lights().get().empty())
			{
				float light_pdf;
				auto light = select_light(sampler->get1D(), &light_pdf);
				auto Ld = direct_illumination(intersection, light, sampler) / light_pdf;
				L += beta * Ld;
			}
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
		if (compMax(beta) < 0.01f && depth > 3)
		{
			float q = std::max(float(.05), 1.f - compMax(beta));
			if (sampler->get1D() < q) break;
			beta /= 1.f - q;
		}

		ray = {o + wi * ray_epsilon, wi};
		depth++;
	}

	return L;
}
