#pragma once

#include <glm/vec3.hpp>
#include "../math/math.h"

namespace pbr
{
	class Fresnel
	{
	public:
		virtual ~Fresnel(){};

		virtual glm::vec3 evaluate(float cos_i, float nt_over_ni) const = 0;

		float eta_i, eta_t;
	};

	class FresnelConductor : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i, float nt_over_ni) const override;

		FresnelConductor(const glm::vec3& eta_n, const glm::vec3& eta_k): eta_n(eta_n), eta_k(eta_k){}

	private:
		glm::vec3 eta_n, eta_k;
	};

	class FresnelDielectric : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i, float nt_over_ni) const override;

		FresnelDielectric(float eta_i, float eta_t) : eta_i(eta_i), eta_t(eta_t){}

		float eta_i, eta_t;
	};

	class FresnelMirror : public Fresnel
	{
	public:
		FresnelMirror() = default;

		glm::vec3 evaluate(float, float) const override;
	};
}
