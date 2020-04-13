#pragma once

#include "shader.h"
#include <map>

namespace rasterizer
{
	class ShaderManager final
	{
	public:
		ShaderManager();

		std::shared_ptr<Shader> reload(int index);

	private:
		std::map<std::string, std::shared_ptr<Shader>> shaders;
	};
}
