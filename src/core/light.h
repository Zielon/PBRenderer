#pragma once

#include <iostream>
#include <utility>

#include "../geometry/scene_object.h"
#include "../parser/types.h"

namespace pbr
{
	class Light
	{
	public:
		virtual ~Light() = default;

		Light(parser::LightConfig config): configuration(std::move(config)){}

		virtual glm::vec3 sample_Li(const Intersection& intersection, const glm::vec2& u, glm::vec3* wi,
		                            float* pdf) const = 0;

		virtual glm::vec3 power() const = 0;

	protected:
		const parser::LightConfig configuration;
	};
}
