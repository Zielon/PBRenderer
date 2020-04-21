#include "ray.h"

pbr::Ray pbr::Ray::spawn(const glm::vec3& dir, const glm::vec3 orig, const glm::vec3& n) const{

	glm::vec3 bias_n = 0.0001f * n;
	bool outside = dot(d, n) < 0;
	return {outside ? orig + bias_n : orig - bias_n, dir};
}
