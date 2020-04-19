#pragma once

#include <memory>

#include "../core/material.h"
#include "../textures/texture.h"

namespace pbr
{
	class LambertianMaterial : public Material
	{
	public:
		void compute(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;

	private:
		std::unique_ptr<Texture<glm::vec3>> kd;
	};
}
