#pragma once

namespace pbr
{
	enum class TransportMode { Radiance, Importance };

	class Intersection;

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void compute_BxDF(Intersection& intersection,
		                          TransportMode mode = TransportMode::Radiance,
		                          bool multiple_lobes = false) const = 0;
	};
}
