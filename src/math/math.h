#include <glm/glm.hpp>
#include <algorithm>
#include "../core/uniform_sampler.h"

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

	inline glm::vec3 spherical_direction(float sinTheta, float cosTheta, float phi){

		return glm::vec3(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
	}

	inline glm::vec3 face_forward(const glm::vec3& v, const glm::vec3& n){

		return (dot(v, n) < 0.f) ? -v : v;
	}

	inline glm::vec3 cosine_sample_hemisphere(const glm::vec2& u){

		glm::vec2 d = pbr::concentric_sample_disk(u);
		float z = std::sqrt(std::max(float(0), 1 - d.x * d.x - d.y * d.y));
		return glm::vec3(d.x, d.y, z);
	}

	inline bool refract(const glm::vec3& wi, const glm::vec3& n, const float& eta, glm::vec3* wt){

		float cosThetaI = dot(n, wi);
		float sin2ThetaI = std::max(float(0), float(1 - cosThetaI * cosThetaI));
		float sin2ThetaT = eta * eta * sin2ThetaI;

		// Handle total internal reflection for transmission
		if (sin2ThetaT >= 1) return false;
		float cosThetaT = std::sqrt(1 - sin2ThetaT);
		*wt = eta * -wi + (eta * cosThetaI - cosThetaT) * n;
		return true;
	}
}
