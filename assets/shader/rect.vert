#version 330 core

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 mvp;

out vec2 vTexCoords;

void main() {
    vTexCoords = aTexCoords;
    gl_Position = mvp * vec4(aPosition, 0.0, 1.0);
}