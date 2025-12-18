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
	/*모델 행렬
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(x, y, z));
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
	glm::mat4 model = trans * scale * rotate;  //결합의 예
	*/
	/*glm::vec3 campos(5, 5, 5);
	glm::vec3 look(0, 0, 0);
	glm::vec3 up(0, 1, 0);

	std::cout << to_string(lookAt(campos, look, up)) << std::endl;
	*/

	glm::vec4 local_point(4.0f, 5.0f, 3.0f, 1.0f);

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, 1.3f, 1.3f));
	glm::mat4 rx = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 ry = glm::rotate(glm::mat4(1.0f), glm::radians(56.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rz = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 model = rx * ry * rz * trans * scale;
	glm::vec4 world = model * local_point;

	std::cout << to_string(world) << std::endl;

	glm::mat4 view = { {},{},{},{} };
	//위에 view 로 부터 카메라위치 campos 랑 카메라가 보고있는 점 look을 어떻게 알지?
	//view 행렬은 world -> camera 변경하는 행렬
	// view = camera -> wolrd 행렬이 됨

	glm::mat4 inview = glm::inverse(view); //camera -> model
	// inview의 마지막 컬럼이 카메라의 위치가됨

	glm::vec3 campos = inview[3]; // 이게 카메라 위치
	//view 구성시, 2개의 축을 구했는데, 카메라는 -z 방향을 바라본다고 가정했음
	

	//카메라는 -z축을 바라보고 있음
	glm::vec3 z_axis = glm::normalize(-inview[2]);

	glm::vec3 look = campos + z_axis + 10.0f;
	glm::vec3 up(0, 1, 0);
	glm::mat4 myview = lookAt(campos, look, up);

	return 0;
}

glm::mat4 lookAt(glm::vec3 campos, glm::vec3 look, glm::vec3 up)
{
	glm::vec3 z = glm::normalize(campos - look);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::normalize(glm::cross(z , x));
	glm::mat4 T = glm::translate(glm::mat4 (1),-campos);
	glm::mat4 R {glm::vec4(x.x, y.x, z.x, 0), glm::vec4(x.y, y.y, z.y, 0), glm::vec4(x.z, y.z, z.z, 0), glm::vec4(0, 0, 0, 1)};

	return R * T;
}