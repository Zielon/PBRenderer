#pragma once

#include "intersectable.h"

namespace pbr
{
	class SceneObject : public Intersectable
	{
	public:
		virtual ~SceneObject() = default;
	};
}
