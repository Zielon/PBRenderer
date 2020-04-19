#include "whitted.h"

#include <chrono>
#include <thread>

#include "../geometry/triangle.h"
#include "../geometry/mesh.h"
#include "../core/uniform_sampler.h"
#include "../math/reflection.h"

glm::vec3 pbr::WhittedIntegrator::Li(const Ray& ray, const std::shared_ptr<Sampler>& sampler, int depth) const{

	glm::vec3 color = glm::vec3(0.235294, 0.67451, 0.843137);

	if (depth >= 6) return color;

	Intersection intersection;

	float bias = 0.00001f;

	if (!scene->intersect(ray, intersection)) return color;

	auto hit = ray.point(intersection.distance);
	const auto triangle = (Triangle*)intersection.triangle;

	if (triangle->scene_object->type == MESH)
	{
		const auto mesh = (Mesh*)triangle->scene_object;

		auto N = intersection.shading.n;

		const parser::MaterialConfig material = mesh->get_config().material;

		if (material.type == "REFLECTION")
		{
			float kr;
			math::fresnel(ray.d, N, material.ior, kr);
			glm::vec3 bias_n = bias * N;
			glm::vec3 reflectionDirection = normalize(math::reflect(ray.d, N));
			bool outside = dot(ray.d, N) < 0;
			glm::vec3 reflectionRayOrig = outside ? hit + bias_n : hit - bias_n;
			glm::vec3 reflectionColor = Li(Ray(reflectionRayOrig, reflectionDirection), sampler, depth + 1);

			color = reflectionColor * material.textures.at("CONSTANT")->evaluate(intersection);
		}
		else if (material.type == "REFLECTION_AND_REFRACTION")
		{
			glm::vec3 refractionColor = glm::vec3(0);

			float kr;
			math::fresnel(ray.d, N, material.ior, kr);
			bool outside = dot(ray.d, N) < 0;
			glm::vec3 bias_n = bias * N;

			if (kr < 1)
			{
				glm::vec3 refractionDirection = normalize(math::refract(ray.d, N, material.ior));
				glm::vec3 refractionRayOrig = outside ? hit - bias_n : hit + bias_n;
				refractionColor = Li(Ray(refractionRayOrig, refractionDirection), sampler, depth + 1);
			}

			glm::vec3 reflectionDirection = normalize(math::reflect(ray.d, N));
			glm::vec3 reflectionRayOrig = outside ? hit + bias_n : hit - bias_n;
			glm::vec3 reflectionColor = Li(Ray(reflectionRayOrig, reflectionDirection), sampler, depth + 1);

			color = reflectionColor * kr + refractionColor * (1 - kr);
		}
		else if (material.type == "LAMBERTIAN")
		{
			glm::vec3 lightAmt = glm::vec3(0);
			glm::vec3 specularColor = glm::vec3(0);
			glm::vec3 shadowPointOrig = (dot(ray.d, N) < 0) ? hit + N * bias : hit - N * bias;

			for (auto& light : scene->get_lights().get())
			{
				glm::vec3 lightDir = light->get_config().position - hit;
				lightDir = normalize(lightDir);
				float LdotN = std::max(0.f, dot(lightDir, N));

				Intersection i;
				bool inShadow = scene->intersect(Ray(shadowPointOrig, lightDir), i);

				lightAmt += (1 - inShadow) * light->get_config().intensity * LdotN;
				glm::vec3 reflectionDirection = math::reflect(-lightDir, N);
				specularColor +=
					powf(std::max(0.f, -dot(reflectionDirection, ray.d)), 25)
					* light->get_config().intensity;
			}

			auto c = glm::vec3(0);

			color = lightAmt * (*material.textures.begin()).second->evaluate(intersection) * 0.8f + specularColor * 0.2f;
		}
	}

	return color;
}
