#pragma once

#include <glm/glm.hpp>

namespace pbr
{
	class Fresnel
	{
	public:
		virtual ~Fresnel(){};

		virtual glm::vec3 evaluate(float cos_i) const = 0;
	};

	/*
	 * Conductors have a complex-valued index of refraction.
	 * Under assumption that refraction for the incident is form a dielectric material.
	 */
	class FresnelConductor : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i) const override;

		FresnelConductor(const glm::vec3& eta_i, const glm::vec3& eta_t, const glm::vec3& k)
			: eta_i(eta_i), eta_t(eta_t), k(k){}

	private:
		glm::vec3 eta_i; // refraction for the incident
		glm::vec3 eta_t; // refraction for the transmitted
		glm::vec3 k; // absorption coefficient
	};

	class FresnelDielectric : public Fresnel
	{
	public:
		glm::vec3 evaluate(float cos_theta_i) const override;

		FresnelDielectric(float eta_i, float eta_t) : eta_i(eta_i), eta_t(eta_t){}

	private:
		float eta_i; // refraction for the incident
		float eta_t; // refraction for the transmitted
	};

	class FresnelMirror : public Fresnel
	{
	public:
		FresnelMirror() = default;

		glm::vec3 evaluate(float) const override;
	};
}
