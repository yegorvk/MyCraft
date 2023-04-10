#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

out vec3 vColor;

uniform mat4 mvp;

void main() {
    vColor = aColor;
    gl_Position = mvp * vec4(aPosition, 1.0);
}