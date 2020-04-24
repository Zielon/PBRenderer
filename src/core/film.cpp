#include "film.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <glm/ext/scalar_constants.hpp>

pbr::Film::Film(glm::vec2 size):
	pixels(std::vector<std::vector<Pixel<float>>>(size.y, std::vector<Pixel<float>>(size.x))), size(size){}

pbr::Pixel<float> pbr::Film::get_pixel(int x, int y){

	if (x >= size.x || y >= size.y)
		throw std::runtime_error("Out of range!");

	return pixels[y][x];
}

void pbr::Film::set_pixel(Pixel<float> pixel, int x, int y){

	if (x >= size.x || y >= size.y)
		throw std::runtime_error("Out of range!");

	pixels[y][x] = pixel;
}

void pbr::Film::set_pixel(glm::vec3 pixel, int x, int y){

	if (x >= size.x || y >= size.y)
		throw std::runtime_error("Out of range!");

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

	std::cout << "INFO::FILM file [" << file << "] saved" << std::endl;

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

	std::cout << "INFO::FILM file [" << file << "] saved" << std::endl;

	ofs.close();
}

inline float gaussian2D(const glm::vec2& sample){

	auto a = 1.f / glm::sqrt(2.f * glm::pi<float>());
	auto x = glm::exp(-(sample.x - 0.5f) * (sample.x - 0.5f) / 2.f);
	auto y = glm::exp(-(sample.y - 0.5f) * (sample.y - 0.5f) / 2.f);

	return a * a * x * y;
}

void pbr::Film::merge(const std::vector<PixelSamples>& pixels){

	const int width = int(size.x);
	const int height = int(size.y);

	#pragma omp parallel num_threads(std::thread::hardware_concurrency())
	{
		#pragma omp for schedule(dynamic, 128)
		for (auto j = 0; j < width * height; ++j)
		{
			const auto y = int(j / width);
			const auto x = j - y * width;

			glm::vec3 pixel{0.f};
			float normalization{0.f};

			for (auto& tuple : pixels[j])
			{
				auto estimated_pixel = std::get<0>(tuple);
				auto sample = std::get<1>(tuple);
				auto pdf = gaussian2D(sample);

				pixel += pdf * estimated_pixel;
				normalization += pdf;
			}

			pixel /= normalization;
			set_pixel(pixel, x, y);
		}
	}
}

glm::vec2 pbr::Film::get_size() const{

	return size;
}
