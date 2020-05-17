#pragma once

#include "../core/light.h"
#include "../core/distribution.h"
#include "../geometry/transformation.h"
#include "../textures/hdr_texture.h"

namespace pbr
{
	class Ray;
	class Intersection;

	class InfiniteLight : public Light
	{
	public:
		explicit InfiniteLight(const parser::LightConfig& config);

		glm::vec3 power() const override;

		glm::vec3 Le(const Ray& ray) const;

		glm::vec3 sample_Li(
			const Intersection& intersection,
			const std::shared_ptr<Scene>& scene,
			const glm::vec2& u,
			glm::vec3* wi,
			float* pdf,
			bool* shadow) const override;

		float pdf_Li(const Intersection& ref, const std::shared_ptr<Scene>& scene, const glm::vec3& wi) const override;

		float sample_Le(
			const Intersection& intersection,
			const glm::vec2& u,
			glm::vec3* wi,
			float* pdf) const override;

	private:
		std::shared_ptr<HDRTexture> hdr;
		std::shared_ptr<Distribution2D> distribution;
		Transformation transformation;
	};
}
