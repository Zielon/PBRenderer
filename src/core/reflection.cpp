#include "reflection.h"

#include <utility>

#include "bxdf.h"
#include <algorithm>
#include "../geometry/intersection.h"
#include "../bxdfs/specular_reflection.h"

pbr::BSDF::BSDF(Intersection& intersection, std::shared_ptr<SceneObject> object):
	n(intersection.shading.n),
	to_local_coordinate(intersection.shading.to_local),
	to_world_coordinate(intersection.shading.to_world),
	object(std::move(object)){ }

void pbr::BSDF::add(std::shared_ptr<BxDF> bxdf){

	bxdfs.push_back(bxdf);
}

glm::vec3 pbr::BSDF::f(const glm::vec3& wo_w, const glm::vec3& wi_w, const BxDFType flags) const{

	glm::vec3 wi = to_local(wi_w);
	glm::vec3 wo = to_local(wo_w);

	if (wo.z == 0) return glm::vec3(0.f);

	const bool reflect = dot(wi_w, n) * dot(wo_w, n) > 0;

	glm::vec3 f(0.f);

	for (const auto& bxdf : bxdfs)
	{
		const bool no_refraction = (reflect && (bxdf->type & REFLECTION)) || (!reflect && (bxdf->type & TRANSMISSION));
		if (bxdf->matches_flags(flags) && no_refraction)
			f += bxdf->f(wo, wi);
	}

	return f;
}

glm::vec3 pbr::BSDF::sample_f(
	const glm::vec3& wo_w, glm::vec3* wi_w, const glm::vec2& u, float* pdf, BxDFType type,
	BxDFType* sampled_type) const{

	const int components = num_components(type);

	if (components == 0)
	{
		*pdf = 0.f;
		if (sampled_type)
			*sampled_type = BxDFType(0);
		return glm::vec3(0.f);
	}

	int comp = std::min(int(std::floor(u[0] * components)), components - 1);

	std::shared_ptr<BxDF> bxdf = nullptr;
	int count = comp;
	for (const auto& b : bxdfs)
		if (b->matches_flags(type) && count-- == 0)
		{
			bxdf = b;
			break;
		}

	glm::vec2 u_remapped(std::min(u[0] * components - comp, 1.f - std::numeric_limits<float>::epsilon()), u[1]);

	glm::vec3 wi = glm::vec3(0.f);
	const glm::vec3 wo = to_local(wo_w);
	if (wo.z == 0) return glm::vec3(0.f);
	if (sampled_type) *sampled_type = bxdf->type;
	*pdf = 0.f;

	glm::vec3 f = bxdf->sample_f(wo, &wi, u_remapped, pdf, sampled_type);

	if (*pdf == 0)
	{
		if (sampled_type)
			*sampled_type = BxDFType(0);
		return glm::vec3(0.f);
	}

	*wi_w = to_world(wi);

	if (!(bxdf->type & SPECULAR) && components > 1)
		for (const auto& b : bxdfs)
			if (b != bxdf && b->matches_flags(type))
				*pdf += b->pdf(wo, wi);

	if (components > 1) *pdf /= components;

	if (!(bxdf->type & SPECULAR))
	{
		bool reflect = dot(*wi_w, n) * dot(wo_w, n) > 0;
		f = glm::vec3(0.f);
		for (const auto& b : bxdfs)
		{
			const bool no_refraction = (reflect && (b->type & REFLECTION)) || (!reflect && (b->type & TRANSMISSION));
			if (b->matches_flags(type) && no_refraction)
				f += b->f(wo, wi);
		}
	}

	return f;
}

int pbr::BSDF::num_components(BxDFType flags) const{

	int num = 0;
	for (const auto& bxdf : bxdfs)
		if (bxdf->matches_flags(flags)) ++num;

	return num;
}

glm::vec3 pbr::BSDF::to_world(const glm::vec3& v) const{

	return to_world_coordinate * v;
}

glm::vec3 pbr::BSDF::to_local(const glm::vec3& v) const{

	return to_local_coordinate * v;
}

float pbr::BSDF::pdf(const glm::vec3& wo, const glm::vec3& wi) const{

	return 0.f;
}
