#include "hdr_texture.h"

#include "../geometry/intersection.h"
#include <stb_image.h>

pbr::HDRTexture::HDRTexture(std::string name, const std::string& path) : Texture<glm::vec3>(name){

	int channel;
	auto* data = stbi_loadf(path.c_str(), &width, &height, &channel, 0);

	pixels.resize(height, std::vector<glm::vec3>(width));
	for (int i = 0, index = 0; i < height; ++i)
		for (int j = 0; j < width; ++j, index += channel)
			pixels[i][j] = glm::vec3(data[index], data[index + 1], data[index + 2]);

	stbi_image_free(data);
}

glm::vec3 pbr::HDRTexture::evaluate(Intersection& intersection) const{

	auto u = intersection.shading.uv.x;
	auto v = intersection.shading.uv.y;

	return pixels[int((height - 1) * v)][int((width - 1) * u)];
}

glm::vec3 pbr::HDRTexture::get(glm::vec2 uv){

	auto u = uv.x;
	auto v = uv.y;

	return pixels[int((height - 1) * v)][int((width - 1) * u)];
}

glm::vec2 pbr::HDRTexture::get_size() const{

	return {width, height};
};
