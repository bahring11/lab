/*
#define GLM_ENABLE_EXPERIMENTAL 
#include "MyGlWindow.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>





static const GLfloat cube_vertices[] = {
	// front
	-1.0, -1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0,
	// back
	-1.0, -1.0, -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0,
};

static const GLfloat cube_colors[] = {
	// front colors
	1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0,
};

static const GLushort cube_elements[] = {
	0, 1, 2,  2, 3, 0,  // front
	1, 5, 6,  6, 2, 1,  // right
	7, 6, 5,  5, 4, 7,  // back
	4, 0, 3,  3, 7, 4,  // left
	4, 5, 1,  1, 0, 4,  // bottom
	3, 2, 6,  6, 7, 3   // top
};


MyGlWindow::MyGlWindow(int w, int h)

//==========================================================================
{
	m_width = w;
	m_height = h;

	// 쉐이더 초기화
	shaderProgram = std::make_unique<ShaderProgram>();
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");


	// 1. Non-DSA 방식 (전통적인 방식)
	// setupBuffer_NonDSA();

	// 2. DSA 방식 (Modern OpenGL 4.5+)
	setupBuffer_DSA();
	
}

// --------------------------------------------------------
// 1. Non-DSA 방식 구현 (Bind -> Edit)
// --------------------------------------------------------
void MyGlWindow::setupBuffer_NonDSA()
{
	// 1. VAO 생성 및 바인딩
	glGenVertexArrays(1, &vaohandle);
	glBindVertexArray(vaohandle);

	// 2. Position VBO
	GLuint vbo_position;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // 3 float per vertex
	glEnableVertexAttribArray(0);

	// 3. Color VBO
	GLuint vbo_color;
	glGenBuffers(1, &vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // 3 float per color
	glEnableVertexAttribArray(1);

	// 4. Element(Index) Buffer
	// 중요: VAO가 바인딩된 상태에서 IBO를 바인딩해야 함
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	// 5. 언바인딩 (선택사항)
	glBindVertexArray(0);
	// 주의: IBO는 VAO 언바인딩 전에 언바인딩하면 안 됩니다.
}

// --------------------------------------------------------
// 2. DSA 방식 구현 (Direct State Access)
// --------------------------------------------------------
void MyGlWindow::setupBuffer_DSA()
{
	// 1. 객체 생성 (Create)
	glCreateVertexArrays(1, &vaohandle);

	GLuint vbo_position, vbo_color, ibo_elements;
	glCreateBuffers(1, &vbo_position);
	glCreateBuffers(1, &vbo_color);
	glCreateBuffers(1, &ibo_elements);

	// 2. 데이터 업로드 (NamedBufferData - 바인딩 불필요)
	glNamedBufferData(vbo_position, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glNamedBufferData(vbo_color, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glNamedBufferData(ibo_elements, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	// 3. VAO 연결 설정

	// --- Position (속성 0) ---
	// (1) 버퍼 연결: VAO Binding Point 0 <-> vbo_position
	glVertexArrayVertexBuffer(vaohandle, 0, vbo_position, 0, sizeof(float) * 3);
	// (2) 포맷 설정: 속성 0은 float 3개
	glVertexArrayAttribFormat(vaohandle, 0, 3, GL_FLOAT, GL_FALSE, 0);
	// (3) 바인딩 연결: 속성 0 <-> Binding Point 0
	glVertexArrayAttribBinding(vaohandle, 0, 0);
	glEnableVertexArrayAttrib(vaohandle, 0);

	// --- Color (속성 1) ---
	// (1) 버퍼 연결: VAO Binding Point 1 <-> vbo_color
	glVertexArrayVertexBuffer(vaohandle, 1, vbo_color, 0, sizeof(float) * 3);
	// (2) 포맷 설정: 속성 1은 float 3개
	glVertexArrayAttribFormat(vaohandle, 1, 3, GL_FLOAT, GL_FALSE, 0);
	// (3) 바인딩 연결: 속성 1 <-> Binding Point 1
	glVertexArrayAttribBinding(vaohandle, 1, 1);
	glEnableVertexArrayAttrib(vaohandle, 1);

	// 4. IBO 연결
	glVertexArrayElementBuffer(vaohandle, ibo_elements);

	// DSA는 마지막에 언바인딩(glBindVertexArray(0))을 할 필요가 없습니다.
}


void MyGlWindow::draw(void)
{
	// 화면 클리어
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 쉐이더 사용
	shaderProgram->use();

	// VAO 바인딩
	glBindVertexArray(vaohandle);

	// 인덱스 개수 계산 및 그리기
	// 방법 1: 매 프레임 GPU에 버퍼 사이즈를 물어보는 방식 (작성하신 코드)
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	// 방법 2 (권장): 우리는 큐브가 36개의 인덱스(삼각형 12개 * 3)임을 알고 있으므로 숫자를 바로 쓰는 것이 성능에 좋습니다.
	// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

	shaderProgram->disable();
}



void MyGlWindow::setupBuffer()
{
	//쉐이더 객체 생성 modern c++를 이용
	shaderProgram = std::make_unique<ShaderProgram>();

	//load shaders : 버텍스 쉐이더랑 프레그먼트 쉐이더를 지정
	shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	//NON-DSA 방법
	
	//삼각형의 정보 배열에 저장; 속성번호 0
	const float vertexPosition[] = {
		-0.2f, 0.0f, 0.0f, 1.0f,
		-0.2f, 0.4f, 0.0f, 1.0f,
		0.2f, 0.0f, 0.0f, 1.0f
		 
	};
	//삼각형의 생삭 지정 속성번호 1
	const float vertexColor[] = {
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f
	};

	const float vertices[] = { // 인터리브 방식
		-0.2f, 0.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f, //position, color for v0
		0.2f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f, //position, color for v1
		0.0f, 0.4f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f //position, color for v2
	};


	/*
	// VAO 생성 및 바인딩
	glGenVertexArrays(1, &vaohandle);
	glBindVertexArray(vaohandle);

	// 1. 위치 VBO 설정 (속성번호 0)
	GLuint vbo_position;
	glGenBuffers(1, &vbo_position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position);

	// **수정 1: sizeof(vertexPosition)으로 수정**
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);

	glVertexAttribPointer( //cpU 한테 
		0, //속성번호
		4, //데이터 수
		GL_FLOAT, //데이터타입
		GL_FALSE, //노말라이즈 여부
		0,
		0
	);
	// **수정 2: 활성화로 수정**
	glEnableVertexAttribArray(0);

	// 2. 색상 VBO 설정 (속성번호 1)
	GLuint vbo_color;
	glGenBuffers(1, &vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);

	// **수정 1: sizeof(vertexColor)로 수정**
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	// **수정 2: 활성화로 수정**
	glEnableVertexAttribArray(1);

	// VBO와 VAO 바인딩 해제
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	



	glGenVertexArrays(1, &vaohandle); //컨테이너 생성
	glBindVertexArray(vaohandle);


	GLuint vbo_vertax;
	glGenBuffers(1, &vbo_vertax);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertax);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, //속성번호 : 0
		4, //버텍스당 데이터수 
		GL_FLOAT,
		GL_FALSE,
		sizeof(float)*7, //위치가 시작한후에 얼만 데이터후에 다시 위치가 시작하냐? : 스트라이드
		(void*) 0 //offset : 얼마 후에 데이터가 시작하냐?
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0, //속성번호 : 0
		3, //버텍스당 데이터수 
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 7, //위치가 시작한후에 얼만 데이터후에 다시 위치가 시작하냐? : 스트라이드 형식
		(void*)(sizeof(float)*4) //offset : 얼마 후에 데이터가 시작하냐?
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	*/

	//DSA 방법 : 함수명이 다름 (gl 바인딩이 없음)
	//1. vao 생성


	/*
	GLuint vbo_position;
	GLuint vbo_color;

	glCreateVertexArrays(1, &vaohandle);
	glCreateBuffers(1, &vbo_position);
	glCreateBuffers(1, &vbo_color);


	//2: .vbo 생성

	//positon 처리

	//CPU 메모리 할당
	glNamedBufferData(vbo_position, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
	//GPU에게 해석방법 알려줌
		glVertexArrayVertexBuffer(
			vaohandle,
			0, //바인딩 인덱스 번호
			vbo_position, //vbo
			0, //offset
			sizeof(float)*4 //스트라이드

		);

	glNamedBufferData(vbo_color, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
		//GPU에게 해석방법 알려줌
		glVertexArrayVertexBuffer(
			vaohandle,
			1, //바인딩 인덱스
			vbo_color, //vbo
			0, //offset
			sizeof(float) * 3 //스트라이드

		);

	glVertexArrayAttribFormat(vaohandle,
			0, //속성번호
			4, //데이터수
			GL_FLOAT,
			GL_FALSE,
			0 //offset
	);
	glVertexArrayAttribBinding(vaohandle,
		0, //속성번호
		0 //바인딩 인덱스
	);

	glEnableVertexArrayAttrib(vaohandle, 0);

	glVertexArrayAttribFormat(vaohandle,
		1, //속성번호
		3, //데이터수
		GL_FLOAT,
		GL_FALSE,
		0 //offset
	);
	glVertexArrayAttribBinding(vaohandle,
		1, //속성번호
		1 //바인딩 인덱스
	);

	glEnableVertexArrayAttrib(vaohandle, 1);
	*/


	/*
	GLuint vbo_vertex;
	glCreateVertexArrays(1, &vaohandle);
	glCreateBuffers(1, &vbo_vertex);

	glNamedBufferData(vbo_vertex, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//GPU에게 해석방법 알려줌
	glVertexArrayVertexBuffer(
		vaohandle,
		0, //바인딩 인덱스 번호
		vbo_vertex, //vbo
		0, //offset
		sizeof(float) * 7 //스트라이드

	);
	glVertexArrayAttribFormat(vaohandle,
		0, //속성번호
		4, //데이터수
		GL_FLOAT,
		GL_FALSE,
		0 //offset
	);
	glVertexArrayAttribBinding(vaohandle,
		0, //속성번호
		0 //바인딩 인덱스
	);
	glEnableVertexArrayAttrib(vaohandle, 0);


	glVertexArrayAttribFormat(vaohandle,
		1, //속성번호
		3, //데이터수
		GL_FLOAT,
		GL_FALSE,
		sizeof(float)*4 //offset
	);
	glVertexArrayAttribBinding(vaohandle,
		1, //속성번호
		0 //바인딩 인덱스
	);
	glEnableVertexArrayAttrib(vaohandle, 1);
	*/

/*
void MyGlWindow::draw(void)
{
	glClearColor(0.2, 0.2, 0.2, 1.0); // 백 그라운드 생성 지정 (R, G, B, A)
	//어떤 버퍼를 가지고 올지 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//0. 캔버스 설정 : 캔버스 킈 및 위치 설정
	// 일부만 쓸건지 결정
	
	//glViewport(0, 0, m_width * 0.5, m_height * 0.5); // 앞 두개 값 : 시작점, 뒤 두개 : 크기

	//쉐이더 use
	shaderProgram->use();
	// vao binding
	glBindVertexArray(vaohandle);

	// cal draw function
	//glDrawArrays(GL_TRIANGLES, 0, 6); // 어떻게 그릴래? 삼각형으로, 시작 인덱스, 버텍스
	// 
	//인덱스가 없을때
	// 트라이앵글스는 3개씩 끊어서 한다
	//glDrawArrays(GL_TRIANGLE_STRIP, 0 ,4); //자동으로  중간에2개는 반복주고 마지막에 버텍스 하나더
	//glDrawArrays(GL_TRIANGLES, 0, 3); //삼각형 그리기


	//인덱스가 있을때
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


	shaderProgram->disable();
}
*/


#include "MyGlWindow.h"
#include <iostream>

// GLM 변환 함수들을 위해 필수적인 헤더들
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 마우스 버튼 상태 상수
const int MOUSE_NONE = 0;
const int MOUSE_LEFT = 1;
const int MOUSE_RIGHT = 2;
const int MOUSE_MIDDLE = 4;

MyGlWindow::MyGlWindow(int w, int h)
	: m_width(w), m_height(h), m_mouseButtonFlags(MOUSE_NONE), m_prevMouseX(0.0), m_prevMouseY(0.0)
{
	// 쉐이더 로드
	try {
		m_program = Program::GenerateFromFileVsFs("shaders/simple.vert", "shaders/simple.frag");
	}
	catch (const std::exception& e) {
		std::cerr << "Shader Load Error: " << e.what() << std::endl;
	}

	// 소(Cow) 객체 생성 (테스트용 큐브 데이터 사용)
	m_cow = std::make_unique<Cow>();

	// 뷰어(카메라) 생성
	float aspect = (float)w / (float)h;
	m_viewer = std::make_unique<Viewer>(
		glm::vec3(0.0f, 0.0f, 30.0f),  // [수정] 거리를 6.0f -> 30.0f 정도로 늘리세요
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		45.0f, aspect
	);
}

void MyGlWindow::setSize(int w, int h)
{
	m_width = w;
	m_height = h;
	if (m_viewer) {
		m_viewer->setAspectRatio((float)w / (float)h);
	}
}

void MyGlWindow::onMouseDown(int button, int action)
{
	if (action == 1) { // Pressed
		if (button == 0) m_mouseButtonFlags |= MOUSE_LEFT;
		if (button == 1) m_mouseButtonFlags |= MOUSE_RIGHT;
		if (button == 2) m_mouseButtonFlags |= MOUSE_MIDDLE;
	}
	else if (action == 0) { // Released
		if (button == 0) m_mouseButtonFlags &= ~MOUSE_LEFT;
		if (button == 1) m_mouseButtonFlags &= ~MOUSE_RIGHT;
		if (button == 2) m_mouseButtonFlags &= ~MOUSE_MIDDLE;
	}
}

void MyGlWindow::onMouseMove(double x, double y)
{
	float deltaX = (float)(x - m_prevMouseX) / (float)m_width;
	float deltaY = (float)(y - m_prevMouseY) / (float)m_height;

	if (m_viewer) {
		if (m_mouseButtonFlags & MOUSE_LEFT) {
			m_viewer->rotate(deltaX, deltaY);
		}
		else if (m_mouseButtonFlags & MOUSE_RIGHT) {
			m_viewer->zoom(deltaY);
		}
		else if (m_mouseButtonFlags & MOUSE_MIDDLE) {
			m_viewer->translate(deltaX, deltaY, true);
		}
	}

	m_prevMouseX = x;
	m_prevMouseY = y;
}

void MyGlWindow::draw()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!m_program) return;

	m_program->BindProgram();

	// 1. Model Matrix
	glm::mat4 model = glm::mat4(1.0f);

	// 2. View Matrix
	glm::mat4 view = glm::mat4(1.0f);
	if (m_viewer) {
		view = glm::lookAt(
			m_viewer->getViewPoint(),
			m_viewer->getViewCenter(),
			m_viewer->getUpVector()
		);
	}

	// 3. Projection Matrix
	// [수정된 부분] w, h 대신 멤버 변수 m_width, m_height 사용
	float aspect = (float)m_width / (float)m_height;

	// 초기값
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

	if (m_viewer) {
		// Viewer의 FOV 값 사용
		projection = glm::perspective(glm::radians(m_viewer->getFieldOfView()), aspect, 0.1f, 100.0f);
	}

	// 4. MVP 전송
	glm::mat4 mvp = projection * view * model;
	m_program->SetMatrix("mvp", mvp);

	// 소(Cow) 그리기
	if (m_cow) {
		m_cow->draw();
	}

	m_program->UnbindProgram();
}