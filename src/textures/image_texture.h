#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <string>
#include <utility>
#include <vector>

#include "texture.h"
#include "../core/film.h"

namespace pbr
{
	class Intersection;

	class Image
	{
	public:
		Image(int width, int height, std::string path, int num_channels):
			pixels(std::vector<std::vector<Pixel<float>>>(height, std::vector<Pixel<float>>(width))),
			num_channels(num_channels), width(width), height(height), path(std::move(path)){

			load();
		}

		void save(const std::string& file);

		Pixel<float> get(float u, float v);

		Pixel<float> get(glm::vec2 uv);

		glm::vec2 get_size() const;

	private:
		std::vector<std::vector<Pixel<float>>> pixels;
		int num_channels;
		int width;
		int height;
		std::string path;

		void load();
	};

	class ImageTexture : public Texture<glm::vec3>
	{
	public:
		ImageTexture(std::string name, std::shared_ptr<Image> image) : Texture(name), image(std::move(image)){}

		ImageTexture(std::string name, int width, int height, std::string path)
			: Texture(name), image(std::make_shared<Image>(width, height, path, 3)){}

		glm::vec3 evaluate(Intersection& intersection) const override;

		glm::vec3 get(glm::vec2 uv) const;

		glm::vec2 get_size() const;

	private:
		std::shared_ptr<Image> image;
	};
}
