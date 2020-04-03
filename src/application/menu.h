#pragma once

#include <functional>
#include <vector>
#include <glm/glm.hpp>

namespace app
{
	class Menu final
	{
	public:
		Menu(const glm::ivec2& position, const glm::ivec2& size);

		void attach(std::function<void()> func);

		void draw() const;

	private:
		glm::ivec2 position;
		glm::ivec2 size;
		std::vector<std::function<void()>> functions;
	};
}
