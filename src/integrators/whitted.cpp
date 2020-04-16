#include "whitted.h"

#include <chrono>
#include <thread>

#include "../geometry/triangle.h"
#include "../geometry/mesh.h"
#include "../core/uniform_sampler.h"
#include "../math/reflection.h"

glm::vec3 pbr::WhittedIntegrator::process(const Ray& ray, int depth) const{

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

		if (material.type == "REFLECTION_AND_REFRACTION")
		{
			glm::vec3 reflectionDirection = normalize(reflect(ray.d, N));
			glm::vec3 refractionDirection = normalize(math::refract(ray.d, N, material.ior));

			glm::vec3 reflectionRayOrig = (dot(reflectionDirection, N) < 0) ? hit - N * bias : hit + N * bias;
			glm::vec3 refractionRayOrig = (dot(refractionDirection, N) < 0) ? hit - N * bias : hit + N * bias;

			glm::vec3 reflectionColor = process(Ray(reflectionRayOrig, reflectionDirection), depth + 1);
			glm::vec3 refractionColor = process(Ray(refractionRayOrig, refractionDirection), depth + 1);

			float kr;
			math::fresnel(ray.d, N, material.ior, kr);
			color = reflectionColor * kr + refractionColor * (1 - kr);
		}
		else if (material.type == "DIFFUSE_AND_GLOSSY")
		{
			glm::vec3 lightAmt = glm::vec3(0);
			glm::vec3 specularColor = glm::vec3(0);
			glm::vec3 shadowPointOrig = (dot(ray.d, N) < 0) ? hit + N * bias : hit - N * bias;

			for (auto& light : scene->get_lights().get())
			{
				glm::vec3 lightDir = light->get_config().position - hit;
				float lightDistance2 = dot(lightDir, lightDir);
				lightDir = normalize(lightDir);
				float LdotN = std::max(0.f, dot(lightDir, N));

				Intersection i;
				bool inShadow = scene->intersect(Ray(shadowPointOrig, lightDir), i);

				lightAmt += (1 - inShadow) * light->get_config().intensity * LdotN;
				glm::vec3 reflectionDirection = reflect(-lightDir, N);
				specularColor +=
					powf(std::max(0.f, -dot(reflectionDirection, ray.d)), material.specular)
					* light->get_config().intensity;
			}

			color = lightAmt * material.color * material.kd + specularColor * material.ks;
		}
	}

	return color;
}
