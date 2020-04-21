#pragma once

#include "reflection.h"

namespace pbr
{
	class BxDF
	{
	public:
		BxDF(BxDFType type) : type(type){}

		virtual ~BxDF() = default;

		virtual glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const = 0;

		virtual glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi,
		                           const glm::vec2& sample, float* pdf,
		                           BxDFType* sampledType = nullptr) const;

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		bool matches_flags(BxDFType t) const;

		const BxDFType type;
	};
}
