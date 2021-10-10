
# GLFW
add_subdirectory(glfw-3.3.4)

include_directories(
	glfw-3.1.2/include/GLFW/
)

# GLAD
add_library(glad glad/include/glad/glad.h glad/src/glad.c)

target_include_directories(glad PUBLIC glad/include/)