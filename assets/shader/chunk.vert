#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexCoords;

out vec3 vTexCoords;

uniform mat4 mvp;

void main() {
    vTexCoords = aTexCoords;

    gl_Position = mvp * vec4(aPosition, 1.0);
}