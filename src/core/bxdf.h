#pragma once

#include "reflection.h"

namespace pbr
{
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

	class BxDF
	{
	public:
		BxDF(BxDFType type) : type(type){}

		virtual ~BxDF() = default;

		virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const = 0;

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		const BxDFType type;
	};

	class LambertianReflection : public BxDF
	{
	public:
		LambertianReflection(const glm::vec3& r): BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), r(r){}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const glm::vec3 r;
	};
}
