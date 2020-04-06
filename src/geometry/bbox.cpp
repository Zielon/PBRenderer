#include "bbox.h"

#include <glm/common.hpp>

pbr::BBox::BBox(const glm::vec3& min, const glm::vec3& max): m_min(min), m_max(max){}

void pbr::BBox::extend(const glm::vec3& vec){

	m_min = glm::min(m_min, vec);
	m_max = glm::max(m_max, vec);
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

bool pbr::BBox::intersect(const Ray& ray, float* hit_t0, float* hit_t1) const{

	return false;
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
