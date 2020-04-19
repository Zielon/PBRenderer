#pragma once

#include <memory>

#include "../core/material.h"
#include "../textures/texture.h"

namespace pbr
{
	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::shared_ptr<Texture> kd): kd(std::move(kd)){}

		void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;

	private:
		std::shared_ptr<Texture> kd;
	};
}
