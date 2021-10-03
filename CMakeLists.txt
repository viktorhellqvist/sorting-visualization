cmake_minimum_required(VERSION 3.10)

project(Sorting-Visualization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# TODO: Get find_package(GLEW) to work
# set(GLEW_ROOT ${DIR} external/glew-2.2.0/)
# set(GLEW_INCLUDE_DIR external/glew-2.2.0/include)
# set(GLEW_LIBRARIES external/glew-2.2.0/lib)

find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)


add_subdirectory(external)

include_directories(
	external/glfw-3.3.4/include/
	external/glm-0.9.9.7/
	external/glew-2.2.0/include
	.
)


# add the executable
add_executable(Sorting-visualization
	main.cpp
)

target_link_libraries(Sorting-visualization
	${OPENGL_LIBRARY}
	glfw
	GLEW_220
)
