#pragma once

#include "../parser/types.h"

namespace pbr
{
	enum class TransportMode { Radiance, Importance };

	class Intersection;

	class Material
	{
	public:
		virtual ~Material() = default;

		static std::shared_ptr<Material> create_material(parser::MaterialConfig& config);

		virtual void compute_BxDF(
			Intersection& intersection,
			TransportMode mode = TransportMode::Radiance,
			bool multiple_lobes = false) const = 0;
	};
}
