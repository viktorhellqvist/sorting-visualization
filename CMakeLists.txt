cmake_minimum_required(VERSION 3.10)

project(Sorting-Visualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(OpenGL REQUIRED)

add_subdirectory(external)

include_directories(
	external/glfw-3.3.4/include/
	external/glm-0.9.9.7/
	.
)

# add the executable
add_executable(Sorting-visualization
	main.cpp
	shaderLoader.cpp
	shaderLoader.h
)

target_link_libraries(Sorting-visualization
	${OPENGL_LIBRARY}
	glfw
	glad
)
