#pragma once

#include <random>
#include <glm/vec2.hpp>

namespace pbr
{
	class Sampler
	{
	public:
		virtual ~Sampler() = default;

		virtual float get1D() = 0;

		virtual glm::vec2 get2D() = 0;
	};

	class UniformSampler : public Sampler
	{
	public:

		UniformSampler(float min = 0.f, float max = 1.f): generator(std::random_device()()), distribution(min, max){}

		float get1D() override{

			return distribution(generator);
		};

		glm::vec2 get2D() override{

			return glm::vec2(distribution(generator), distribution(generator));
		};

	private:
		std::mt19937 generator;
		std::uniform_real_distribution<float> distribution;
	};
}
