#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp> //∞≥≥‰¿˚¿∏∑Œ 2x3 
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/dual_quaternion.hpp>

glm::mat4 PersPective(float fovY, float aspect, float near, float far)
{
	fovY = glm::radians(fovY);
	glm::mat4 PS(0.0f);
	PS[0][0] = 1.0f / (aspect * glm::tan(fovY / 2.0f));
	PS[1][1] = 1.0f / (glm::tan(fovY / 2.0f));
	PS[2][2] = -(far + near) / (far - near);
	PS[2][3] = -1.0f;
	PS[3][2] = -(2.0f * far * near / (far - near));
	return PS;
}

int main()
{
	float w = 780.0f;
	float h = 750.0f;
	float forY = 45.0f;
	forY = glm::radians(forY);
	float nearZ = 0.1f;
	float farZ = 500.0f;
	float aspect = w / h;
	glm::mat4 PS = PersPective(forY, aspect, nearZ, farZ);
	std::cout << glm::to_string(PS) << std::endl;
	return 0;
}

