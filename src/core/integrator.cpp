#include "integrator.h"

#define NOMINMAX

#include <omp.h>
#include <thread>
#include <algorithm>
#include <utility>
#include <windows.h>

#include "uniform_sampler.h"
#include "../cameras/projective.h"
#include "../geometry/mesh.h"
#include "../math/math.h"

pbr::Integrator::Integrator(std::shared_ptr<Scene> scene, int num_samples, std::string name):
	name(std::move(name)), num_samples(num_samples), scene(std::move(scene)){

	const auto threads = std::thread::hardware_concurrency();
	for (auto i = 0; i < threads; ++i)
		samplers.push_back(std::make_shared<HammersleySampler>());
}

void pbr::Integrator::render(float& progress){

	const auto start = std::chrono::steady_clock::now();

	const auto film_size = get_camera()->get_film()->get_size();
	const int width = int(film_size.x);
	const int height = int(film_size.y);

	std::vector<PixelSamples> pixels(height * width, PixelSamples());
	const auto patch_size = 256; // 16x16
	const auto fraction = float(patch_size) / float(height * width);

	#pragma omp parallel num_threads(std::thread::hardware_concurrency())
	{
		#pragma omp for schedule(dynamic)
		for (auto s = 0; s < height * width; s += patch_size)
		{
			for (auto j = s; j < s + patch_size; ++j)
			{
				if (j >= height * width) break;

				const auto y = int(j / width);
				const auto x = j - y * width;
				auto& sampler = samplers[omp_get_thread_num()];
				auto pixel = get_film()->get_pixel(x, y).to_vec3();

				for (auto i = 0; i < num_samples; i++)
				{
					const auto weight = 1.0f / (i + 1);
					const auto offset =
						std::dynamic_pointer_cast<HammersleySampler, Sampler>(sampler)->get2D();
					auto ray = get_camera()->cast_ray(glm::vec2(x, y), offset);

					pixel *= i * weight;
					pixel += weight * Li(ray, sampler, 0);
				}

				get_film()->set_pixel(pixel, x, y);
			}

			progress += fraction;
		}
	}

	const auto end = std::chrono::steady_clock::now();
	const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//get_film()->merge(pixels);
	get_film()->save_jpg(name + "_" + std::to_string(num_samples) + "_" + std::to_string(millis) + ".jpg");

	std::cout << "INFO::INTEGRATOR (" << name << ") render time: [" << millis << " ms]" << std::endl;
}

std::shared_ptr<pbr::Light> pbr::Integrator::select_light(float u, float* pdf) const{

	auto n_lights = int(scene->get_lights().get().size());
	auto selected = std::min(int(u * n_lights), n_lights - 1);
	*pdf = 1.f / n_lights;
	return scene->get_lights().get()[selected];
}

inline float power_heuristic(int nf, float fPdf, int ng, float gPdf){

	auto f = nf * fPdf;
	auto g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

/*
 *  Sample light source and scattered direction with multiple importance sampling
 */
glm::vec3 pbr::Integrator::direct_illumination(
	const Intersection& intersection,
	const std::shared_ptr<Light>& light,
	const std::shared_ptr<Sampler>& sampler) const{

	auto wo = intersection.wo;
	auto o = intersection.point;
	auto ns = intersection.shading.n;

	glm::vec3 Ld(0.f);

	auto bsdf_flags = BxDFType(ALL & ~SPECULAR);
	glm::vec3 wi{0.f};
	float light_pdf{0.f};
	float scattering_pdf{0.f};
	bool is_shadow{};

	// === 1 - sample light source ===
	auto Li = light->sample_Li(intersection, scene, sampler->get2D(), &wi, &light_pdf, &is_shadow);

	if (light_pdf > 0.f && Li != glm::vec3(0.f))
	{
		auto f = intersection.bsdf->f(wo, wi, bsdf_flags) * std::abs(dot(wi, ns));
		scattering_pdf = intersection.bsdf->pdf(wo, wi, bsdf_flags);

		if (f != glm::vec3(0.f))
		{
			if (is_shadow) Li = glm::vec3(0.f);

			// Add light's contribution to reflected radiance
			if (Li != glm::vec3(0.f))
			{
				float weight = power_heuristic(1, light_pdf, 1, scattering_pdf);
				Ld += f * Li * weight / light_pdf;
			}
		}
	}

	// === 2 - sample scattered direction for surface interactions ===
	BxDFType sampled_type;
	auto f = intersection.bsdf->sample_f(wo, &wi, sampler, &scattering_pdf, bsdf_flags, &sampled_type);
	f *= std::abs(dot(wi, ns));
	auto is_specular = (sampled_type & SPECULAR) != 0;

	if (scattering_pdf > 0.f && f != glm::vec3(0.f))
	{
		// Account for light contributions along sampled direction wi
		float weight = 1.f;
		if (!is_specular)
		{
			light_pdf = light->pdf_Li(intersection, scene, wi);
			if (light_pdf == 0) return Ld;
			weight = power_heuristic(1, scattering_pdf, 1, light_pdf);
		}

		// Find intersection with light to account if a new sampled direction from BxDF
		// intersects an area light and accumulate its contribution
		Intersection light_intersection;
		Ray surface_ray = {o + wi * ray_epsilon, wi};
		auto intersects = scene->intersect(surface_ray, light_intersection);

		Li = glm::vec3(0.f);

		if (intersects)
		{
			auto mesh = dynamic_cast<Mesh*>(light_intersection.triangle->scene_object);
			if (mesh->type == LIGHT && mesh->get_area_light() == light)
				Li = mesh->get_area_light()->L(light_intersection.shading.n, -wi);
			if (Li != glm::vec3(0.f))
				Ld += f * Li * weight / scattering_pdf;
		}
	}

	return Ld;
}

glm::vec3 pbr::Integrator::reflect(
	const Ray& ray, const std::shared_ptr<Sampler>& sampler, Intersection& isect, int depth) const{

	float pdf;
	glm::vec3 wi;

	auto wo = isect.wo;
	auto o = isect.point;
	auto ns = isect.shading.n;
	auto error = isect.error;
	auto f = isect.bsdf->sample_f(wo, &wi, sampler, &pdf, BxDFType(REFLECTION | SPECULAR));

	Ray reflection{math::offset_ray_origin(o, ns, error, wi), wi};

	if (pdf > 0.f && f != glm::vec3(0.f) && glm::abs(dot(wi, ns)) != 0.f)
		return f * Li(reflection, sampler, depth + 1) * std::abs(dot(wi, ns)) / pdf;

	return glm::vec3(0.f);
}

glm::vec3 pbr::Integrator::transmit(
	const Ray& ray, const std::shared_ptr<Sampler>& sampler, Intersection& isect, int depth) const{

	float pdf;
	glm::vec3 wi;

	auto wo = isect.wo;
	auto o = isect.point;
	auto ns = isect.shading.n;
	auto error = isect.error;
	auto f = isect.bsdf->sample_f(wo, &wi, sampler, &pdf, BxDFType(TRANSMISSION | SPECULAR));

	Ray refraction{math::offset_ray_origin(o, ns, error, wi), wi};

	if (pdf > 0.f && f != glm::vec3(0.f) && glm::abs(dot(wi, ns)) != 0.f)
		return f * Li(refraction, sampler, depth + 1) * std::abs(dot(wi, ns)) / pdf;

	return glm::vec3(0.f);
}

std::shared_ptr<pbr::Film> pbr::Integrator::get_film() const{

	return scene->get_camera()->get_film();
}

std::shared_ptr<pbr::Camera> pbr::Integrator::get_camera() const{

	return scene->get_camera();
}
