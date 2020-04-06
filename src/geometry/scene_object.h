#pragma once

#include "interactable.h"

namespace pbr
{
	class SceneObject : public Interactable
	{
	public:
		virtual ~SceneObject() = default;
	};
}
