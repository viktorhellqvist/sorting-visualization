#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 outColor;

void main () {
    gl_Position = vec4(vertexPosition, 1);
    outColor = vertexColor;
}