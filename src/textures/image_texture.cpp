#include "image_texture.h"

#include <stb_image.h>
#include <algorithm>

#include "../geometry/intersection.h"

void pbr::Image::load(){

	auto* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);

	pixels.resize(height, std::vector<Pixel<float>>(width));

	for (int i = 0, index = 0; i < height; ++i)
		for (int j = 0; j < width; ++j, index += num_channels)
			pixels[i][j] = glm::vec3(data[index], data[index + 1], data[index + 2]) / 255.f;

	stbi_image_free(data);
}

inline float clamp(const float& lo, const float& hi, const float& v){

	return std::max(lo, std::min(hi, v));
}

pbr::Pixel<float> pbr::Image::get(float u, float v){

	return pixels[int((height - 1) * v)][int((width - 1) * u)];
}

pbr::Pixel<float> pbr::Image::get(glm::vec2 uv){

	return get(uv.x, uv.y);
}

glm::vec2 pbr::Image::get_size() const{

	return glm::vec2(width, height);
}

glm::vec3 pbr::ImageTexture::evaluate(Intersection& intersection) const{

	return image->get(intersection.shading.uv.x, intersection.shading.uv.y).to_vec3();
}

glm::vec3 pbr::ImageTexture::get(glm::vec2 uv) const{

	return image->get(uv).to_vec3();
}

glm::vec2 pbr::ImageTexture::get_size() const{

	return image->get_size();
}
