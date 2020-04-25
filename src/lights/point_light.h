#pragma once

#include "../core/light.h"

namespace pbr
{
	class PointLight final : public Light
	{
	public:
		PointLight(const parser::LightConfig& config)
			: Light(POINT, config),
			  position(config.position),
			  intensity(config.intensity){}

		glm::vec3 sample_Li(
			const Intersection& intersection, const std::shared_ptr<Scene>& scene, const glm::vec2& u, glm::vec3* wi, float* pdf, bool* shadow) const override;

		float pdf_Li(const Intersection& ref, const glm::vec3& wi) const override;

		float sample_Le(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		glm::vec3 power() const override;

	private:
		const glm::vec3 position;
		const glm::vec3 intensity;
	};
}
