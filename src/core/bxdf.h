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

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		const BxDFType type;
	};
}
