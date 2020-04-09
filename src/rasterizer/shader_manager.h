#pragma once

#include "shader.h"
#include <map>

namespace rasterizer
{
	class Shader_manager final
	{
	public:
		Shader_manager();

		std::shared_ptr<Shader> reload(int index);

	private:
		std::map<std::string, std::shared_ptr<Shader>> shaders;
	};
}
