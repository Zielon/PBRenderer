#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

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
		explicit BSDF(Intersection& intersection);

		void add(std::shared_ptr<BxDF> bx);

		virtual float pdf(const glm::vec3& wo, const glm::vec3& wi) const;

		/*
		 * This function assumes that light is decoupled in different wavelengths.
		 */
		glm::vec3 f(const glm::vec3& woW, const glm::vec3& wiW, BxDFType flags = ALL) const;

		glm::vec3 sample_f(const glm::vec3& wo, glm::vec3* wi, const glm::vec2& u,
		                   float* pdf, BxDFType type = ALL,
		                   BxDFType* sampledType = nullptr) const;

	private:
		std::vector<std::shared_ptr<BxDF>> bxdfs;
	};
}
