#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


int main()
{
	const double DEGREES_TO_RADIANS = glm::pi<float>() / 180.0;
	const double RADIANS_TO_DEGREES = 180.0 / glm::pi<float>();

	//다음 두백터의 내적 내적의값은 스칼라
	//내적을 구하는 명령어 dot
	//glm::vec3 v(7.0f, 3.0f, -2.0f);
	//glm::vec3 u(10.0f, 4.0f, 2.0f);
	//float d = glm::dot(v, u);
	//std::cout << d << std::endl;
	
	//glm::vec3 a(2.0f, -1.0f, 1.0f); 
	//glm::vec3 b(1.0f, 1.0f, 2.0f);

	//float c = glm::dot(a, b); //c는 백터의 내적
	//float d = c / (glm::length(a) * glm::length(b));
	//d는 백터의 내적을 백터의 길이로 나눈것
	//float e = glm::acos(d) * RADIANS_TO_DEGREES;
	//std::cout << e << std::endl;

	//glm::vec3 v(2, 0, 0);
	//glm::vec3 u(-1, 1, 0);

	//v = glm::normalize(v);
	//u = glm::normalize(u);

	//float d = glm::dot(v, u);
	//std::cout << d << std::endl;

	//glm::vec3 v(1, 2, 3);
	//glm::vec3 c(1, 3, 5); //c

	//float l = glm::dot(v, c);
	//glm::vec3 b = c * l;
	//glm::vec3 a = v - b;

	//std::cout << to_string(b) << to_string(b) << std::endl;

	glm::vec3 n(0, 1, 0);
	glm::vec3 p(1, 0, 1);
	glm::vec3 x(-3, 3, 2);

	glm::vec3 v = x - p;
	n = glm::normalize(n);
	float d = glm::dot(v, u);

	return 0;
}