#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "../geometry/scene_object.h"

namespace pbr
{
	class Intersection;
	class BxDF;

	enum BxDFType
	{
		REFLECTION = 1 << 0,
		TRANSMISSION = 1 << 1,
		DIFFUSE = 1 << 2,
		GLOSSY = 1 << 3,
		SPECULAR = 1 << 4,
		ALL = DIFFUSE | GLOSSY | SPECULAR | REFLECTION | TRANSMISSION,
	};

	/**
	 * BxDF Manager which calculates overall scattering for all registered BxDFs
	 */
	class BSDF
	{
	public:
		explicit BSDF(Intersection& intersection, std::shared_ptr<SceneObject> object);

		void add(std::shared_ptr<BxDF> bx);

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		/*
		 * This function assumes that light is decoupled in different wavelengths.
		 */
		glm::vec3 f(const glm::vec3& wo_w, const glm::vec3& wi_w, BxDFType flags = ALL) const;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& u,
		                   float* pdf, BxDFType type = ALL,
		                   BxDFType* sampled_type = nullptr) const;

		int num_components(BxDFType flags = ALL) const;

	private:
		std::vector<std::shared_ptr<BxDF>> bxdfs;
		std::shared_ptr<SceneObject> object;
		glm::vec3 n;
		glm::mat3 to_local_coordinate;
		glm::mat3 to_world_coordinate;

		glm::vec3 to_world(const glm::vec3& v) const;

		glm::vec3 to_local(const glm::vec3& v) const;
	};
}
