#pragma once
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <vector>

class Cow
{
public:
	Cow();
	~Cow();
	void draw();

private:
	void setupBuffer_DSA();
	void calculateNormal();

	GLuint vaohandle;
	GLuint vbo_cow_vertices;
	GLuint vbo_cow_normals;
	GLuint ibo_cow_elements;

	std::vector<glm::vec3> normals;
	int m_indexCount;
};