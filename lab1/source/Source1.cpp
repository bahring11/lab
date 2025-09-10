#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

int main()
{
	//����1��
	glm::vec3 p(1.0f, 4.0f, -5.0f);
	glm::vec3 v(5.0f, 1.0f, 4.0f);
	p = p + v;
	std::cout << "1�� �� " << glm::to_string(p) << std::endl;
	//����2��
	glm::vec3 v1(1.0f, 0.0f, 0.0f);
	glm::vec3 v2(0.0f, 1.0f, 0.0f);
	v1 = v1 + v2;
	std::cout << "2�� �� " << glm::to_string(v1) << std::endl;
	//����3��
	glm::vec3 v3(0.0f, 1.0f, 0.0f);
	v3 = v3 * 2.0f;
	std::cout << "3�� �� " << glm::to_string(v3) << std::endl;
	//����4��
	glm::vec3 p1(8.0f, 2.0f, 5.0f);
	glm::vec3 p2(1.0f, -1.0f, 4.0f);
	p1 = p2 - p1;
	std::cout << "4�� �� " << glm::to_string(p1) << std::endl;
	//����5��
	glm::vec3 v4(1.0f, 3.0f, 4.0f);
	v4 = glm::normalize(v4);
	std::cout << "5�� �� " << glm::to_string(v4) << std::endl;
	//����6��
	glm::vec3 v5(1.0f, -1.0f, 4.0f);
	glm::vec4 v6(v5, 0.0f);
	std::cout << "6�� �� " << glm::to_string(v6) << std::endl;
	return 0;
}