#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexCoords;
layout (location = 2) in vec3 aColor;

out vec3 vTexCoords;
out vec3 vColor;

uniform mat4 mvp;

void main() {
    vTexCoords = aTexCoords;
    vColor = aColor;

    gl_Position = mvp * vec4(aPosition, 1.0);
}