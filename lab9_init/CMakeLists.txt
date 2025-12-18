# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# [중요] C++ 표준 설정은 프로젝트 생성 전에 하는 것이 가장 좋습니다.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(PROJECT_NAME lab)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)

# source 폴더의 모든 cpp, h 파일을 SOURCES 변수에 담습니다.
file(GLOB_RECURSE SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/source")

# Add source to this project's executable.
# [수정] colorCube.cpp/h는 이미 ${SOURCES}에 포함되어 있으므로 중복해서 적지 않습니다.
add_executable(${PROJECT_NAME} ${SOURCES} "source/cow.cpp" "source/cowvbo.h" "source/cow_data.h")
add_executable(${PROJECT_NAME} ${SOURCES} "source/Viewer.h" "source/Viewer.cpp")
add_executable(${PROJECT_NAME} ${SOURCES} "source/Program.h" "source/Program.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glfw unofficial::gl3w::gl3w)