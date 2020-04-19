#pragma once

#include <glm/glm.hpp>

namespace pbr
{
	class Intersection;

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
		BSDF(Intersection& intersection);

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi, const glm::vec2& sample, float& pdf,
		            BxDFType type = BSDF_ALL);

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;
	};
}
