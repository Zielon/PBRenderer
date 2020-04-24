#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <mutex>

using PixelSamples = std::vector<std::tuple<glm::vec3, glm::vec2>>;

namespace pbr
{
	template <typename T>
	struct Pixel
	{
		Pixel() = default;

		Pixel(glm::vec3 color): r(color.x), g(color.y), b(color.z){}

		Pixel(glm::vec4 color) : r(color.x), g(color.y), b(color.z), a(color.w){}

		Pixel(T r, T g, T b): r(r), g(g), b(b){}

		Pixel(T r, T g, T b, T a) : r(r), g(g), b(b), a(a){}

		T r{}, g{}, b{}, a = 0.f;
		T filter_weight_sum = 0.f;

		float operator[](int i) const{

			return to_vec4()[i];
		};

		float& operator[](int i){

			return to_vec4()[i];
		};

		glm::vec<4, T> to_vec4(){

			return glm::vec<4, T>(r, g, b, a);
		}

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

		void merge(const std::vector<PixelSamples>& pixels);

		glm::vec2 get_size() const;

	private:
		std::vector<std::vector<Pixel<float>>> pixels;
		glm::vec2 size{};
		std::mutex mtx;
	};
}
