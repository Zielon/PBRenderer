#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace app
{
	class Window final
	{
	public:
		Window(int width, int height);

		~Window();

		GLFWwindow* get() const;

	private:
		GLFWwindow* window{nullptr};
		const int width;
		const int height;
	};
}
