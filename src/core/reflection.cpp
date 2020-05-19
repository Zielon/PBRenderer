#include "reflection.h"

#include <algorithm>

#include "bxdf.h"
#include "../core/uniform_sampler.h"
#include "../geometry/intersection.h"

pbr::BSDF::BSDF(Intersection& intersection, SceneObject* object, float eta) :
	n(normalize(intersection.shading.n)),
	s_tangent(normalize(intersection.shading.dpdu)),
	t_bitangent(cross(n, s_tangent)),
	point(intersection.point),
	object(object){}

void pbr::BSDF::add(const std::shared_ptr<BxDF>& bxdf){

	bxdfs.push_back(bxdf);
}

glm::vec3 pbr::BSDF::f(const glm::vec3& wo_w, const glm::vec3& wi_w, const BxDFType flags) const{

	glm::vec3 wi = vertex_to_local(wi_w);
	glm::vec3 wo = vertex_to_local(wo_w);

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

/*
 * Sampling BSDFs from the density that is the average of their individual densities.
 */
glm::vec3 pbr::BSDF::sample_f(
	const glm::vec3& wo_w,
	glm::vec3* wi_w,
	const std::shared_ptr<Sampler>& sampler,
	float* pdf,
	BxDFType type,
	BxDFType* sampled_type) const{

	const int components = num_components(type);

	if (components == 0)
	{
		*pdf = 0.f;
		if (sampled_type)
			*sampled_type = BxDFType(0);
		return glm::vec3(0.f);
	}

	// Use 1D sample probability to determine one component with equal probability.
	const int component = std::min(int(std::floor(sampler->get1D() * components)), components - 1);

	std::shared_ptr<BxDF> bxdf = nullptr;
	int count = component;
	for (const auto& b : bxdfs)
		if (b->matches_flags(type) && count-- == 0)
		{
			bxdf = b;
			break;
		}

	glm::vec3 wi = glm::vec3(0.f);
	const glm::vec3 wo = vertex_to_local(wo_w);
	if (wo.z == 0) return glm::vec3(0.f);

	*sampled_type = bxdf->type;
	*pdf = 0.f;

	glm::vec3 f = bxdf->sample_f(wo, &wi, sampler->get2D(), pdf, sampled_type);

	if (*pdf == 0)
	{
		if (sampled_type)
			*sampled_type = BxDFType(0);
		return glm::vec3(0.f);
	}

	*wi_w = normalize(vertex_to_world(wi));

	/*
	 * Skipped if the chosen BxDF is perfectly specular, since the PDF has an implicit delta distribution in it.
	 * It would be incorrect to add the other PDF values to this one,
	 * since it is a delta term represented with the value 1, rather than as an actual delta distribution.
	 */
	if (!(bxdf->type & SPECULAR) && components > 1)
		for (const auto& b : bxdfs)
			if (b != bxdf && b->matches_flags(type))
				// Because *pdf already holds the PDF value for the distribution the sample was taken from, 
				// we only need to add in the contributions of the others.
				*pdf += b->pdf(wo, wi);

	if (components > 1)
		*pdf /= components;

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

glm::vec3 pbr::BSDF::vertex_to_world(const glm::vec3& v) const{

	return glm::vec3(
		s_tangent.x * v.x + t_bitangent.x * v.y + n.x * v.z,
		s_tangent.y * v.x + t_bitangent.y * v.y + n.y * v.z,
		s_tangent.z * v.x + t_bitangent.z * v.y + n.z * v.z
	);
}

glm::vec3 pbr::BSDF::vertex_to_local(const glm::vec3& v) const{

	return glm::vec3(dot(v, s_tangent), dot(v, t_bitangent), dot(v, n));
}

float pbr::BSDF::pdf(const glm::vec3& wo_w, const glm::vec3& wi_w, BxDFType flags) const{

	glm::vec3 wi = vertex_to_local(wi_w);
	glm::vec3 wo = vertex_to_local(wo_w);
	if (wo.z == 0) return 0.f;
	auto pdf{0.f};
	auto matching = 0;

	for (const auto& bxdf : bxdfs)
		if (bxdf->matches_flags(flags))
		{
			pdf += bxdf->pdf(wo, wi);
			matching++;
		}

	return matching > 0 ? pdf / matching : 0.f;
}
