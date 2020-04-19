#pragma once

#include "../core/bxdf.h"

namespace pbr
{
	class LambertianReflection : public BxDF
	{
	public:
		LambertianReflection(const glm::vec3& r) : BxDF(BxDFType(REFLECTION | DIFFUSE)), r(r){}

		glm::vec3 f(const glm::vec3& wo, const glm::vec3& wi) const override;

	private:
		const glm::vec3 r;
	};
}
