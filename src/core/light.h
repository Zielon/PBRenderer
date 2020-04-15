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
		Light(parser::LightConfig config): configuration(std::move(config)){}

		parser::LightConfig get_config() const{

			return configuration;
		}

	protected:
		const parser::LightConfig configuration;
	};
}
