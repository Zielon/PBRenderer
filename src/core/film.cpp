#include "film.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <algorithm>
#include <fstream>

pbr::Film::Film(glm::vec2 size):
	pixels(std::vector<std::vector<Pixel<float>>>(size.y, std::vector<Pixel<float>>(size.x))), size(size){}

pbr::Pixel<float> pbr::Film::get_pixel(int x, int y){

	return pixels[y][x];
}

void pbr::Film::set_pixel(Pixel<float> pixel, int x, int y){

	pixels[y][x] = pixel;
}

void pbr::Film::set_pixel(glm::vec3 pixel, int x, int y){

	pixels[y][x] = Pixel<float>(pixel.x, pixel.y, pixel.z);
}

inline float clamp(const float& lo, const float& hi, const float& v){

	return std::max(lo, std::min(hi, v));
}

void pbr::Film::save_jpg(const std::string& file){

	const auto output = new char[size.x * size.y * 3];
	auto index = 0;

	for (int j = 0; j < size.y; ++j)
	{
		for (int i = 0; i < size.x; ++i)
		{
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].r));
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].g));
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].b));
		}
	}

	stbi_write_jpg(file.c_str(), size.x, size.y, 3, output, 100);

	delete[] output;
}

void pbr::Film::save_ppm(const std::string& file){

	const auto output = new char[size.x * size.y * 3];
	auto index = 0;

	std::ofstream ofs(file, std::ios::out | std::ios::binary);

	ofs << "P6\n" << size.x << " " << size.y << "\n255\n";
	for (int j = 0; j < size.y; ++j)
	{
		for (int i = 0; i < size.x; ++i)
		{
			const char r = char(255 * clamp(0, 1, pixels[j][i].r));
			const char g = char(255 * clamp(0, 1, pixels[j][i].g));
			const char b = char(255 * clamp(0, 1, pixels[j][i].b));
			ofs << r << g << b;
		}
	}

	ofs.close();
}
