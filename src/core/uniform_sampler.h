#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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

	class HammersleySampler : public UniformSampler
	{
	public:
		glm::vec2 hammersley(uint32_t i, uint32_t N) const{

			return glm::vec2(float(i) / float(N), radical_inverse_vdc(i));
		}

	private:
		static float radical_inverse_vdc(uint32_t bits){

			bits = (bits << 16u) | (bits >> 16u);
			bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
			bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
			bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
			bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

			return 2.3283064365386963e-10 * float(bits);
		}
	};

	inline glm::vec2 uniform_sample_disk(const glm::vec2& u){

		float r = std::sqrt(u[0]);
		float theta = 2 * glm::pi<float>() * u[1];

		return glm::vec2(r * std::cos(theta), r * std::sin(theta));
	}

	inline glm::vec2 concentric_sample_disk(const glm::vec2& u){

		// Map uniform random numbers to [-1,1]^2
		glm::vec2 u_offset = 2.f * u - glm::vec2(1, 1);

		if (u_offset.x == 0 && u_offset.y == 0) return glm::vec2(0, 0);
		float theta, r;

		if (std::abs(u_offset.x) > std::abs(u_offset.y))
		{
			r = u_offset.x;
			theta = glm::quarter_pi<float>() * (u_offset.y / u_offset.x);
		}
		else
		{
			r = u_offset.y;
			theta = glm::half_pi<float>() - glm::quarter_pi<float>() * (u_offset.x / u_offset.y);
		}

		return r * glm::vec2(std::cos(theta), std::sin(theta));
	}
}
