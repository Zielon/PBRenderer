#include "bbox.h"

#include <glm/common.hpp>

pbr::BBox::BBox(const glm::vec3& min, const glm::vec3& max): m_min(min), m_max(max){}

void pbr::BBox::extend(const glm::vec3& vec){

	m_min = glm::min(m_min, vec);
	m_max = glm::max(m_max, vec);
}

void pbr::BBox::extend(const glm::vec3& min, const glm::vec3& max){

	m_min = glm::min(m_min, min);
	m_max = glm::max(m_max, max);
}

void pbr::BBox::extend(const BBox& bbox){

	m_min = glm::min(m_min, bbox.m_min);
	m_max = glm::max(m_max, bbox.m_max);
}

glm::vec3 pbr::BBox::diagonal() const{

	return m_max - m_min;
}

int pbr::BBox::maximum_extent() const{

	const auto d = diagonal();
	return d.z > d.y && d.z > d.x ? 2 : (d.y > d.x);
}

float pbr::BBox::volume() const{

	const auto d = diagonal();
	return d.x * d.y * d.z;
}

float pbr::BBox::surface_area() const{

	const auto d = diagonal();
	return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
}

glm::vec3 pbr::BBox::centroid() const{

	return m_min * 0.5f + m_max * 0.5f;
}

glm::vec3 pbr::BBox::min() const{

	return m_min;
}

glm::vec3 pbr::BBox::max() const{

	return m_max;
}

bool pbr::BBox::intersect(const Ray& ray, const glm::vec3& inv_dir) const{

	const auto t0 = (m_min - ray.o) * inv_dir;
	const auto t1 = (m_max - ray.o) * inv_dir;

	const auto min = glm::min(t0, t1);
	const auto max = glm::max(t0, t1);

	const auto t_min = glm::max(min.x, glm::max(min.y, min.z));
	const auto t_max = glm::min(max.x, glm::min(max.y, max.z));

	return t_max >= t_min;
}
