#pragma once

#include "../core/light.h"

namespace pbr
{
	class PointLight final : public Light
	{
	public:
		PointLight(const parser::LightConfig& config)
			: Light(config),
			  position(config.position),
			  intensity(config.intensity){}

		glm::vec3 sample_Li(
			const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const override;

		glm::vec3 power() const override;

	private:
		const glm::vec3 position;
		const glm::vec3 intensity;
	};
}
