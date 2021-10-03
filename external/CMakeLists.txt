
# GLFW
add_subdirectory(glfw-3.3.4)

include_directories(
	glfw-3.1.2/include/GLFW/
	glew-2.2.0/include/
)

add_library(GLEW_220 STATIC
	glew-2.2.0/src/glew.c
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_220
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)