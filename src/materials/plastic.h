#pragma once

#include "../core/material.h"

namespace pbr
{
	class PlasticMaterial : public Material
	{
	public:
		PlasticMaterial() = default;

		void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const override;
	};
}
