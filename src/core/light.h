#pragma once

#include <utility>
#include <glm/vec2.hpp>

#include "../geometry/object.h"
#include "../parser/types.h"

namespace pbr
{
	class Light : public Object
	{
	public:
		virtual ~Light() = default;

		Light(parser::LightConfig config): Object(LIGHT, config.id), configuration(std::move(config)){}

		virtual glm::vec3 power() const = 0;

		virtual glm::vec3 sample_Li(
			const Intersection& intersection, const glm::vec2& u, glm::vec3* wi, float* pdf) const = 0;

	protected:
		const parser::LightConfig configuration;
	};
}
