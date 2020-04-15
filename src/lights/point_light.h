#pragma once

#include "../core/light.h"

namespace pbr
{
	class PointLight final : public Light
	{
	public:
		explicit PointLight(const parser::LightConfig& config)
			: Light(config){}
	};
}
