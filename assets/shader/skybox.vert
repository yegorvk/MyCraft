#version 330 core

layout (location = 0) in vec3 position;

out vec3 vTexCoords;

uniform mat4 mvp;

void main() {
    vTexCoords = position;
    gl_Position = mvp * vec4(position, 1.0);
}