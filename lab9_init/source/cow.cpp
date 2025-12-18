#include "cowvbo.h"
#include "cow_data.h" // 소 데이터가 들어있는 헤더
#include <iostream>

Cow::Cow()
{
	calculateNormal();
	setupBuffer_DSA();
}

Cow::~Cow()
{
	glDeleteBuffers(1, &vbo_cow_vertices);
	glDeleteBuffers(1, &vbo_cow_normals);
	glDeleteBuffers(1, &ibo_cow_elements);
	glDeleteVertexArrays(1, &vaohandle);
}

void Cow::calculateNormal()
{
	// cow_data.h의 변수명(vertice, nvertices)을 사용해 크기 자동 계산
	int numVertices = sizeof(vertice) / sizeof(glm::vec3);
	int numIndices = sizeof(nvertices) / sizeof(uint32_t);
	int ntris = numIndices / 3;

	normals.resize(numVertices, glm::vec3(0.0f));

	for (int i = 0; i < ntris; ++i) {
		// 인덱스 배열 이름: nvertices
		uint32_t idx0 = nvertices[i * 3];
		uint32_t idx1 = nvertices[i * 3 + 1];
		uint32_t idx2 = nvertices[i * 3 + 2];

		// 안전장치: 인덱스가 정점 배열 범위를 넘어가면 무시 (크래시 방지)
		if (idx0 >= numVertices || idx1 >= numVertices || idx2 >= numVertices) continue;

		// 정점 배열 이름: vertice
		glm::vec3 v0 = vertice[idx0];
		glm::vec3 v1 = vertice[idx1];
		glm::vec3 v2 = vertice[idx2];

		glm::vec3 n = glm::cross(v1 - v0, v2 - v0);
		if (glm::length(n) > 0.00001f) {
			n = glm::normalize(n);
		}

		normals[idx0] = n;
		normals[idx1] = n;
		normals[idx2] = n;
	}
}

void Cow::setupBuffer_DSA()
{
	m_indexCount = sizeof(nvertices) / sizeof(uint32_t);

	glCreateVertexArrays(1, &vaohandle);
	glCreateBuffers(1, &vbo_cow_vertices);
	glCreateBuffers(1, &vbo_cow_normals);
	glCreateBuffers(1, &ibo_cow_elements);

	// 1. Position (vertice 사용)
	glNamedBufferData(vbo_cow_vertices, sizeof(vertice), vertice, GL_STATIC_DRAW);

	// 2. Normal
	glNamedBufferData(vbo_cow_normals, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	// 3. Index (nvertices 사용)
	glNamedBufferData(ibo_cow_elements, sizeof(nvertices), nvertices, GL_STATIC_DRAW);

	// VAO 연결
	// Position (속성 0)
	glVertexArrayVertexBuffer(vaohandle, 0, vbo_cow_vertices, 0, sizeof(float) * 3);
	glVertexArrayAttribFormat(vaohandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaohandle, 0, 0);
	glEnableVertexArrayAttrib(vaohandle, 0);

	// Normal (속성 1)
	glVertexArrayVertexBuffer(vaohandle, 1, vbo_cow_normals, 0, sizeof(float) * 3);
	glVertexArrayAttribFormat(vaohandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaohandle, 1, 1);
	glEnableVertexArrayAttrib(vaohandle, 1);

	// IBO 연결
	glVertexArrayElementBuffer(vaohandle, ibo_cow_elements);
}

void Cow::draw()
{
	glBindVertexArray(vaohandle);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}