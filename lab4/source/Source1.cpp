#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp> //개념적으로 2x3 


int main()
{
	/*
	// 행렬
	glm::vec4 v1 = { 1, 0, 1, -1 };
	glm::vec4 v2 = { 1, 0, 1, -1 };
	glm::vec4 v3 = { 1, 0, 1, -1 };
	glm::vec4 v4 = { 1, 0, 1, -1 };

	glm::mat4 m; //glm::mat3 -> 3x3
	m[0] = v1;
	m[1] = v2;
	m[2] = v3;
	m[3] = v4;

	std::cout << to_string(m) << std::endl;


	glm::mat2 m2 = { 1, 0, 1, -1 };
	std::cout << to_string(m2) << std::endl;

	glm::mat3x2 m3 = { {1,0}, {1,-1}, {0, 1} };
	std::cout << to_string(m3) << std::endl;

	glm::mat4 m4(1.0f); //identity matrix
	std::cout << to_string(m4) << std::endl;

	std::cout << m4[2][2] << std::endl;
	
	//행렬 더하기
	glm::mat2 m1 = { 1, 0, 1, -1 };
	glm::mat2 m2 = { 0, 1, -1, 0 };
	glm::mat2 m3 = m1 + m2;
	std::cout << to_string(m1) << std::endl;
	std::cout << to_string(m2) << std::endl;
	std::cout << to_string(m3) << std::endl;

	//행렬 스칼라곱하기
	glm::mat2 m4 = { 1, 0, 1, -1 };
	m4 = 2.0f * m4;
	std::cout << to_string(m4) << std::endl;
	

	//핼렬 곱셈
	glm::mat2 m1 = { 1, 0, 1, -1 };
	glm::mat2 m2 = { 0, 1, 1, -1 };
	glm::mat2 m3 = m1 * m2;
	std::cout << to_string(m3) << std::endl;


	// 페들렛 문제

	glm::mat3 m1 = { {1, 1, -1},{0,2,0},{2, 3, 1} };
	glm::mat3 m2 = { {1, 0, 0}, {0, -1, 1}, {0, 1, 2} };
	glm::mat3 m3 = m1 * m2;
	glm::mat3 m4 = m2 * m1;

	std::cout << to_string(m3) << std::endl;
	std::cout << to_string(m4) << std::endl;

	*/

	//단위 행렬 identity martix
	//행렬 m 단위 행렬i 의 곱은 항상 m
	// im = mi = m
	// 단위행렬
	glm::mat4(1.0);

	//역행렬 
	//glm::inverse(A)
	/*
	glm::mat2 m2 = { 1, 0, 1, -1 };
	glm::mat2 im = glm::inverse(m2);

	std::cout << to_string(im) << std::endl;

	glm::mat2 ii = m2 * im;
	std::cout << to_string(ii) << std::endl;
	*/

	glm::vec3 a(1, 2, 3);
	glm::mat3 v{ {1, 1, 1},{1, 1, 1},{1, 1, 1} };
	glm::mat3 p = a * v;
	return 0;
}