//#define GLFW_INCLUDE_GLU

#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "MyGlWindow.h"
// #include "Loader.h" // Loader.h가 없으면 주석 처리하거나 빼셔도 됩니다.
#include <memory>

// 전역 스마트 포인터
std::unique_ptr<MyGlWindow> win;

// [추가된 함수 1] 마우스 클릭 이벤트를 받아서 MyGlWindow로 넘겨줌
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (win) {
        win->onMouseDown(button, action);
    }
}

// [추가된 함수 2] 마우스 이동 이벤트를 받아서 MyGlWindow로 넘겨줌
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (win) {
        win->onMouseMove(xpos, ypos);
    }
}

// [추가된 함수 3] 창 크기가 변할 때 비율을 다시 맞춰줌
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // OpenGL 뷰포트 수정
    glViewport(0, 0, width, height);

    // MyGlWindow에도 알려줌 (Viewer 비율 업데이트용)
    if (win) {
        win->setSize(width, height);
    }
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    // 오픈지엘 버전 지정
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 혹은 4.5 등 사용하시는 버전에 맞게

    // Core Profile 사용
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800;
    int height = 800;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // 컨텍스트 생성
    glfwMakeContextCurrent(window);

    // gl3w 초기화
    if (gl3wInit()) {
        fprintf(stderr, "failed to initialize OpenGL\n");
        return -1;
    }

    // 버전 출력
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    // [중요] 콜백 함수 등록 (이 코드가 있어야 마우스가 작동합니다!)
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // MyGlWindow 객체 생성
    win = std::make_unique<MyGlWindow>(width, height);

    // V-Sync 설정
    glfwSwapInterval(1);

    // 메인 루프
    while (!glfwWindowShouldClose(window))
    {
        // 그리기
        if (win) {
            win->draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // 스마트 포인터라 자동으로 해제되지만, 명시적으로 리셋하거나 놔둬도 됨
    // win.reset(); 

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}