#include "fresnel.h"

inline glm::vec3 conductor(float cos_theta_i, const glm::vec3& eta_i, const glm::vec3& eta_t, const glm::vec3& k){

	cos_theta_i = math::clamp(cos_theta_i, -1.f, 1.f);
	glm::vec3 eta = eta_t / eta_i;
	glm::vec3 etak = k / eta_i;

	float cosThetaI2 = cos_theta_i * cos_theta_i;
	float sinThetaI2 = 1.f - cosThetaI2;
	glm::vec3 eta2 = eta * eta;
	glm::vec3 etak2 = etak * etak;

	glm::vec3 t0 = eta2 - etak2 - sinThetaI2;
	glm::vec3 a2plusb2 = sqrt(t0 * t0 + 4.f * eta2 * etak2);
	glm::vec3 t1 = a2plusb2 + cosThetaI2;
	glm::vec3 a = sqrt(0.5f * (a2plusb2 + t0));
	glm::vec3 t2 = float(2) * cos_theta_i * a;
	glm::vec3 Rs = (t1 - t2) / (t1 + t2);

	glm::vec3 t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
	glm::vec3 t4 = t2 * sinThetaI2;
	glm::vec3 Rp = Rs * (t3 - t4) / (t3 + t4);

	return 0.5f * (Rp + Rs);
}

inline glm::vec3 dielectric(float cos_theta_i, float eta_i, float eta_t){

	cos_theta_i = math::clamp(cos_theta_i, -1, 1);

	const bool entering = cos_theta_i > 0.f;

	if (!entering)
	{
		std::swap(eta_i, eta_t);
		cos_theta_i = std::abs(cos_theta_i);
	}

	const float sin_theta_i = std::sqrt(std::max(float(0), 1 - cos_theta_i * cos_theta_i));
	const float sin_theta_t = eta_i / eta_t * sin_theta_i;

	// Total internal reflection
	if (sin_theta_t >= 1) return glm::vec3(1);

	float cos_theta_t = std::sqrt(std::max(float(0), 1 - sin_theta_t * sin_theta_t));
	float rparl = ((eta_t * cos_theta_i) - (eta_i * cos_theta_t)) / ((eta_t * cos_theta_i) + (eta_i * cos_theta_t));
	float rperp = ((eta_i * cos_theta_i) - (eta_t * cos_theta_t)) / ((eta_i * cos_theta_i) + (eta_t * cos_theta_t));

	return glm::vec3((rparl * rparl + rperp * rperp) / 2.f);
}

glm::vec3 pbr::FresnelConductor::evaluate(float cos_theta_i) const{

	return conductor(cos_theta_i, eta_i, eta_t, k);
}

glm::vec3 pbr::FresnelDielectric::evaluate(float cos_theta_i) const{

	return dielectric(cos_theta_i, eta_i, eta_t);
}
