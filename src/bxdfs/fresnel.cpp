#include "fresnel.h"

inline glm::vec3 conductor(float costheta, const glm::vec3& nt_over_ni, const glm::vec3& kt_over_ki){

	/*From https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations*/
	auto cos2theta = costheta * costheta;
	auto sin2theta = 1.0f - cos2theta;
	auto n2 = nt_over_ni * nt_over_ni;
	auto k2 = kt_over_ki * kt_over_ki;

	auto t0 = n2 - k2 - sin2theta;
	auto a2_plus_b2 = glm::sqrt(t0 * t0 + 4.0f * n2 * k2);
	auto t1 = a2_plus_b2 + cos2theta;
	auto a = glm::sqrt(0.5f * (a2_plus_b2 + t0));
	auto t2 = 2.0f * a * costheta;
	auto rs = (t1 - t2) / (t1 + t2);

	auto t3 = cos2theta * a2_plus_b2 + sin2theta * sin2theta;
	auto t4 = t2 * sin2theta;
	auto rp = rs * (t3 - t4) / (t3 + t4);

	return 0.5f * (rp + rs);
}

inline float dielectric(float cos_theta_i, float nt_over_ni){

	auto g = nt_over_ni * nt_over_ni - 1.0f + cos_theta_i * cos_theta_i;

	//In the case of total internal reflection, return 1.
	if (g < 0.0f)
	{
		return 1.0f;
	}

	g = glm::sqrt(g);
	auto g_minus_c = g - cos_theta_i;
	auto g_plus_c = g + cos_theta_i;
	auto temp_div = (cos_theta_i * g_plus_c - 1.0f) / (cos_theta_i * g_minus_c + 1.0f);

	return 0.5f * (g_minus_c * g_minus_c) * (1.0f + temp_div * temp_div) / (g_plus_c * g_plus_c);
}

glm::vec3 pbr::FresnelConductor::evaluate(float cos_theta_i, float nt_over_ni) const{

	return conductor(cos_theta_i, eta_n, eta_k);
}

glm::vec3 pbr::FresnelDielectric::evaluate(float cos_theta_i, float nt_over_n) const{

	return glm::vec3(dielectric(cos_theta_i, nt_over_n));
}

glm::vec3 pbr::FresnelMirror::evaluate(float, float) const{

	return glm::vec3(1.f);
}
