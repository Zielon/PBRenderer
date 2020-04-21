#pragma once

namespace pbr
{
	enum Type { MESH, LIGHT, NONE };

	class Object
	{
	public:
		Object(Type type, int id): id(id), type(type){}

		virtual ~Object() = default;

		int id{};
		Type type = NONE;
	};
}
