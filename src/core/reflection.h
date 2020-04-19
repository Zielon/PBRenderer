#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace pbr
{
	class Intersection;
	class BxDF;

	enum BxDFType
	{
		BSDF_REFLECTION = 1 << 0,
		BSDF_TRANSMISSION = 1 << 1,
		BSDF_DIFFUSE = 1 << 2,
		BSDF_GLOSSY = 1 << 3,
		BSDF_SPECULAR = 1 << 4,
		BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
		BSDF_TRANSMISSION,
	};

	class BSDF
	{
	public:
		explicit BSDF(Intersection& intersection);

		void add(std::shared_ptr<BxDF> bx);

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		glm::vec3 f(const glm::vec3& woW, const glm::vec3& wiW, BxDFType flags = BSDF_ALL) const;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& u,
		                   float* pdf, BxDFType type = BSDF_ALL,
		                   BxDFType* sampledType = nullptr) const;

	private:
		std::vector<std::shared_ptr<BxDF>> bxdfs;
	};
}
