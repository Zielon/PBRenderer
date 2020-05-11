#include "image_texture.h"

#include <stb_image.h>
#include <stb_image_write.h>
#include <algorithm>

#include "../geometry/intersection.h"

void pbr::Image::load(){

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);

	#pragma omp for schedule(dynamic)
	for (auto i = 0; i < width; ++i)
		for (auto j = 0; j < height; ++j)
		{
			Pixel<float> pixel;

			unsigned char* offset = data + (i + width * j) * num_channels;

			pixel.r = offset[0] / 255.0f;
			pixel.g = offset[1] / 255.0f;
			pixel.b = offset[2] / 255.0f;
			pixel.a = num_channels >= 4 ? offset[3] : 0xff / 255.0f;

			pixels[j][i] = pixel;
		}

	stbi_image_free(data);
}

inline float clamp(const float& lo, const float& hi, const float& v){

	return std::max(lo, std::min(hi, v));
}

void pbr::Image::save(const std::string& file){

	const auto output = new char[width * height * 3];
	auto index = 0;

	#pragma omp for schedule(dynamic)
	for (int j = 0; j < height; ++j)
		for (int i = 0; i < width; ++i)
		{
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].r));
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].g));
			output[index++] = char(255 * clamp(0, 1, pixels[j][i].b));
		}

	stbi_write_jpg(file.c_str(), width, height, 3, output, 100);

	delete[] output;
}

pbr::Pixel<float> pbr::Image::get(float u, float v){

	return pixels[int((height - 1) * v)][int((width - 1) * u)];
}

pbr::Pixel<float> pbr::Image::get(glm::vec2 uv){

	return get(uv.x, uv.y);
}

glm::vec3 pbr::ImageTexture::evaluate(Intersection& intersection) const{

	return image->get(intersection.shading.uv.x, intersection.shading.uv.y).to_vec3();
}
