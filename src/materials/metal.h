#pragma once

#include "../core/material.h"

namespace pbr
{
	class MetalMaterial : public Material
	{
	public:
		MetalMaterial() = default;

		void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;
	};
}
