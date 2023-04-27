#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexCoord;

out vec3 vTexCoord;

uniform mat4 mvp;

void main() {
    vTexCoord = aTexCoord;

    gl_Position = mvp * vec4(aPosition, 1.0);
}