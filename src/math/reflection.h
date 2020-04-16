#include <glm/glm.hpp>
#include <algorithm>

namespace math
{
	inline float clamp(const float& lo, const float& hi, const float& v){

		return std::max(lo, std::min(hi, v));
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
