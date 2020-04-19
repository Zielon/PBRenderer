#pragma once

namespace pbr
{
	enum class TransportMode { Radiance, Importance };

	class Intersection;

	class Material
	{
	public:
		virtual ~Material() = default;

		virtual void compute_BxDF(Intersection& intersection, TransportMode mode, bool multiple_lobes) const = 0;
	};
}
