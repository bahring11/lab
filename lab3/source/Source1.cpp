#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


int main()
{
	const double DEGREES_TO_RADIANS = glm::pi<float>() / 180.0;
	const double RADIANS_TO_DEGREES = 180.0 / glm::pi<float>();

	/*glm::vec3 u(1, 0, 0);
	glm::vec3 v(-1, 1, 0);

	glm::vec3 c = glm::cross(u, v);

	float l = glm::length(c);
	float ang = glm::acos(glm::dot(u, v) / glm::length(u) * glm::length(v));

	float l2 = glm::length(u) * glm::length(v) * sin(ang);

	glm::vec3 u2(1, 0, 0);
	glm::vec3 v2(0, 1, 0);
	float area = glm::length(glm::cross(u2, v2));

	glm::vec3 u3(1, 0, 0);
	glm::vec3 v3(1, 0, 0);
	float area = glm::length(glm::cross(u3, v3));
	glm::vec3 u3(1, 0, 0);*/

	//»ï°¢ÇüÀÇ ¸éÀû
	/*glm::vec3 p1(1. 0, 0);
	glm::vec3 p2(1, 1, 0);
	glm::vec3 p3(-1, 0, 0);

	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	float area = 0.5 * glm::length(glm::cross(v1, v2));*/
	
	//»ï°¢ÇüÀÇ ³ë¸» º¤ÅÍ

	glm::vec3 p1(3, 0, 0);
	glm::vec3 p2(1.5, 0.86, 0);
	glm::vec3 p3(3, 0, -1);

	glm::vec3 v1 = p3 - p2;
	glm::vec3 v2 = p1 - p2;
	glm::vec3 n = glm::normalize(glm::cross(v1, v2));

	std::cout << to_string(n) << std::endl;

	
	return 0;	

}