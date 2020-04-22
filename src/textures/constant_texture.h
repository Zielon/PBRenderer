#pragma once

#include "texture.h"

namespace pbr
{
	template <typename T>
	class ConstantTexture : public Texture<T>
	{
	public:

		ConstantTexture(std::string name, T value): Texture<T>(name), value(value){}

		T evaluate(Intersection& intersection) const override;

	private:
		T value{};
	};

	template <typename T>
	T ConstantTexture<T>::evaluate(Intersection& intersection) const{

		return value;
	}
}
