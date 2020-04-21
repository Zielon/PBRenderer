#pragma once

#include <glm/vec3.hpp>
#include "../math/math.h"

namespace pbr
{
	class Fresnel
	{
	public:
		virtual ~Fresnel(){};

		virtual glm::vec3 evaluate(float cos_i) const = 0;
	};

	class FresnelConductor : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i) const override;

		FresnelConductor(const glm::vec3& eta_i, const glm::vec3& eta_t, const glm::vec3& k)
			: eta_i(eta_i), eta_t(eta_t), k(k){}

	private:
		glm::vec3 eta_i, eta_t, k;
	};

	class FresnelDielectric : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i) const override;

		FresnelDielectric(float eta_i, float eta_t) : eta_i(eta_i), eta_t(eta_t){}

	private:
		float eta_i, eta_t;
	};

	class FresnelMirror : public Fresnel
	{
	public:
		FresnelMirror() = default;

		glm::vec3 evaluate(float) const override{

			return glm::vec3(1.f);
		}
	};
}
