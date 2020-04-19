#include <glm/glm.hpp>
#include <algorithm>

namespace math
{
	inline float clamp(const float& lo, const float& hi, const float& v){

		return std::max(lo, std::min(hi, v));
	}

	inline glm::vec3 reflect(const glm::vec3& I, const glm::vec3& N){

		return I - 2 * dot(I, N) * N;
	}

	inline float cos_theta(const glm::vec3& w){

		return w.z;
	}

	inline float cos2_theta(const glm::vec3& w){

		return w.z * w.z;
	}

	inline float abs_cos_theta(const glm::vec3& w){

		return std::abs(w.z);
	}

	inline float sin2_theta(const glm::vec3& w){

		return std::max(float(0), float(1) - cos2_theta(w));
	}

	inline float sin_theta(const glm::vec3& w){

		return std::sqrt(sin2_theta(w));
	}

	inline float tan_theta(const glm::vec3& w){

		return sin_theta(w) / cos_theta(w);
	}

	inline float tan2_theta(const glm::vec3& w){

		return sin2_theta(w) / cos2_theta(w);
	}

	inline float cos_phi(const glm::vec3& w){

		const float theta = sin_theta(w);
		return (theta == 0) ? 1 : clamp(w.x / theta, -1, 1);
	}

	inline float sin_phi(const glm::vec3& w){

		const float theta = sin_theta(w);
		return (theta == 0) ? 0 : clamp(w.y / theta, -1, 1);
	}

	inline float cos2_phi(const glm::vec3& w){

		return cos_phi(w) * cos_phi(w);
	}

	inline float sin2_phi(const glm::vec3& w){

		return sin_phi(w) * sin_phi(w);
	}

	inline float cos_d_phi(const glm::vec3& wa, const glm::vec3& wb){

		return clamp(
			(wa.x * wb.x + wa.y * wb.y) / std::sqrt((wa.x * wa.x + wa.y * wa.y) *
				(wb.x * wb.x + wb.y * wb.y)),
			-1, 1);
	}

	inline bool same_hemisphere(const glm::vec3& w, const glm::vec3& wp){

		return w.z * wp.z > 0;
	}

	inline glm::vec3 refract(const glm::vec3& I, const glm::vec3& N, const float& ior){

		float cosi = clamp(-1, 1, dot(I, N));
		float etai = 1, etat = ior;
		glm::vec3 n = N;
		if (cosi < 0)
		{
			cosi = -cosi;
		}
		else
		{
			std::swap(etai, etat);
			n = -N;
		}
		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);
		return k < 0 ? glm::vec3(0) : eta * I + (eta * cosi - sqrtf(k)) * n;
	}

	inline void fresnel(const glm::vec3& I, const glm::vec3& N, const float& ior, float& kr){

		float cosi = clamp(-1, 1, dot(I, N));
		float etai = 1, etat = ior;
		if (cosi > 0)
		{
			std::swap(etai, etat);
		}
		// Compute sini using Snell's law
		float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
		// Total internal reflection
		if (sint >= 1)
		{
			kr = 1;
		}
		else
		{
			float cost = sqrtf(std::max(0.f, 1 - sint * sint));
			cosi = fabsf(cosi);
			float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
			float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
			kr = (Rs * Rs + Rp * Rp) / 2;
		}
	}
}
