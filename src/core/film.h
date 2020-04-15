#pragma once
#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace pbr
{
	template <typename T>
	struct Pixel
	{
		Pixel() = default;

		Pixel(T r, T g, T b): r(r), g(g), b(b){}

		T r{}, g{}, b = 0.f;
		T filter_weight_sum = 0.f;

		glm::vec<3, T> to_vec3(){

			return glm::vec<3, T>(r, g, b);
		}
	};

	class Film
	{
	public:
		Film(glm::vec2 size);

		Pixel<float> get_pixel(int x, int y);

		void set_pixel(Pixel<float> pixel, int x, int y);

		void set_pixel(glm::vec3 pixel, int x, int y);

		void save_jpg(const std::string& file);

		void save_ppm(const std::string& file);

		glm::vec2 get_size() const;

	private:
		std::vector<std::vector<Pixel<float>>> pixels;
		glm::vec2 size{};
	};
}
