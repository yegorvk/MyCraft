#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUv;

out vec3 vColor;
out vec2 vUv;

uniform mat4 mvp;

void main() {
    vUv = aUv;

    gl_Position = mvp * vec4(aPosition, 1.0);
}