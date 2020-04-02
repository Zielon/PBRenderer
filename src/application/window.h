#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

namespace pbr
{
	class Window
	{
	public:
		Window(int width, int height);

		~Window();

		// Getters and setters

		GLFWwindow* get() const{
			return window;
		}

	private:
		GLFWwindow* window{nullptr};
		const int width;
		const int height;
	};
}
