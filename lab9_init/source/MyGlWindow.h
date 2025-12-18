#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Program.h"
#include "cowvbo.h"     // [수정] Cow.h 포함
#include "Viewer.h"

class MyGlWindow
{
public:
	MyGlWindow(int w, int h);
	~MyGlWindow() = default;
	void draw();
	void setSize(int w, int h);
	void onMouseDown(int button, int action);
	void onMouseMove(double x, double y);

private:
	int m_width;
	int m_height;

	std::unique_ptr<Program> m_program;
	std::unique_ptr<Cow> m_cow; // [수정] Cow 객체 사용 [cite: 655]
	std::unique_ptr<Viewer> m_viewer;

	double m_prevMouseX = 0.0;
	double m_prevMouseY = 0.0;
	int m_mouseButtonFlags = 0;
};