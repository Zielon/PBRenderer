#include "infinite_light.h"

#include <glm/gtc/constants.hpp>
#include <glm/ext/matrix_transform.inl>

#include "../geometry/intersection.h"
#include "../geometry/mesh.h"

pbr::InfiniteLight::InfiniteLight(const parser::LightConfig& config):
	Light(INFINITE, config), hdr(config.hdr),
	transformation(Transformation(config.rotation, config.scaling, config.translation)){

	auto size = hdr->get_size();
	int width = size.x;
	int height = size.y;

	std::vector<std::vector<float>> luminosity(width, std::vector<float>(height));

	/**
	 * Transform the continuously defined spectral radiance function 
	 * defined by the environment map’s texels to a piecewise-constant 
	 * scalar function by computing its luminance at a set of sample points
	 */
	#pragma omp for schedule(dynamic)
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; ++j)
		{
			const glm::vec2 uv((i + 0.5f) / float(width), (j + 0.5f) / float(height));
			auto rgb = hdr->get(uv) * glm::sin(uv.y * glm::pi<float>()); // Map to the sphere
			luminosity[i][j] = dot(glm::vec3(0.212671f, 0.715160f, 0.072169f), rgb); // Luminosity function
		}

	distribution = std::make_shared<Distribution2D>(luminosity);
}

glm::vec3 pbr::InfiniteLight::power() const{

	return {};
}

glm::vec3 pbr::InfiniteLight::Le(const Ray& ray) const{

	auto wi = normalize(transformation.vector_to_local(ray.d));
	auto theta = glm::atan(glm::sqrt(wi.x * wi.x + wi.y * wi.y), wi.z);
	auto phi = glm::atan(wi.y, wi.x) + (wi.y < 0.0f ? glm::two_pi<float>() : 0.0f);

	glm::vec2 uv(phi * glm::one_over_two_pi<float>(), theta * glm::one_over_pi<float>());
	return hdr->get(uv);
}

glm::vec3 pbr::InfiniteLight::sample_Li(const Intersection& intersection, const std::shared_ptr<Scene>& scene,
                                        const glm::vec2& u, glm::vec3* wi, float* pdf, bool* shadow) const{

	auto width = hdr->get_size().x;
	auto height = hdr->get_size().y;
	auto ij = distribution->sample(u);
	auto i = ij.first;
	auto j = ij.second;
	glm::vec2 uv((i + 0.5f) / width, (j + 0.5f) / height);

	auto theta = uv.y * glm::pi<float>();
	auto phi = 2 * uv.x * glm::pi<float>();
	auto sin_theta = glm::sin(theta);

	auto light_space = glm::vec3(glm::cos(phi) * sin_theta, glm::sin(phi) * sin_theta, glm::cos(theta));
	auto map_pdf = width * height * distribution->get_pdf(i, j);

	*wi = normalize(transformation.vector_to_world(light_space));
	*pdf = map_pdf / (2 * glm::pi<float>() * glm::pi<float>() * sin_theta); // Transforming between Distributions

	Intersection shadow_test{};
	*shadow = !scene->intersect(Ray(intersection.point + *wi * ray_epsilon, *wi), shadow_test);

	return hdr->get(uv);
}

float pbr::InfiniteLight::pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene,
                                 const glm::vec3& w) const{

	auto width = hdr->get_size().x;
	auto height = hdr->get_size().y;
	auto wi = transformation.vector_to_local(w);
	auto theta = glm::atan(glm::sqrt(wi.x * wi.x + wi.y * wi.y), wi.z);
	auto phi = glm::atan(wi.y, wi.x) + (wi.y < 0.0f ? glm::two_pi<float>() : 0.0f);
	auto sin_theta = glm::sin(theta);
	glm::vec2 uv(phi * glm::one_over_two_pi<float>(), theta * glm::one_over_pi<float>());

	auto i = static_cast<int>(uv.x * width - 0.5f);
	auto j = static_cast<int>(uv.y * height - 0.5f);
	auto map_pdf = width * height * distribution->get_pdf(i, j);

	return map_pdf / (2 * glm::pi<float>() * glm::pi<float>() * sin_theta);
}

float pbr::InfiniteLight::sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi,
                                    float* pdf) const{

	return 0.f;
}
