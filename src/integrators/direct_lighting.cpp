#include "direct_lighting.h"

#include "../geometry/mesh.h"

glm::vec3 pbr::DirectLighting::Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

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

	float light_pdf;
	auto light = select_light(sampler->get1D(), &light_pdf);
	L += direct_illumination(intersection, light, sampler) / light_pdf;

	if (depth + 1 < max_depth)
	{
		L += reflect(ray, sampler, intersection, depth);
		L += transmit(ray, sampler, intersection, depth);
	}

	return L;
}
