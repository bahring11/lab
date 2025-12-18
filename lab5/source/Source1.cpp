#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp> //개념적으로 2x3 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>


int main()
{
	//동차좌표
	/*
	glm::vec3 v(5, 6, 1);
	glm::mat3 t(1.0);
	t = glm::translate(t, glm::vec2(2, 3));
	v = t * v;
	std::cout << to_string(t) << std::endl;
	std::cout << to_string(v) << std::endl;
	
	//크기조절 행렬
	glm::vec3 v(5, 6, 1);
	glm::mat3 t(1.0);
	t = glm::scale(t, glm::vec2(3, 4)); //scale 3x in x axis and 4x y axis
	v = t * v;
	std::cout << to_string(t) << std::endl;
	std::cout << to_string(v) << std::endl;
	*/
	//2차원 회전 행렬
	glm::vec3 v(5, 6, 1);
	glm::mat3 t(1.0);
	t - glm::rotate(t, glm::radians(45.0f));
	//45 or 45.0 causes an error

	v = t * v;
	std::cout << to_string(t) << std::endl;
	std::cout << to_string(v) << std::endl;

	return 0;
}