#pragma once

namespace pbr
{
	class Triangle
	{
	public:
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 n) : v0(v0), v1(v1), v2(v2), n(n){};

		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;
		glm::vec3 n;
	};
}
