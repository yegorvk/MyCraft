#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aTexCoords;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

out vec3 vViewPos;
out vec3 vTexCoords;
out vec3 vColor;
out vec3 vNormal;

uniform mat4 mvp;
//uniform mat4 modelView;

void main() {
    vTexCoords = aTexCoords;
    vColor = aColor;
    vNormal = aNormal;

    gl_Position = mvp * vec4(aPosition, 1.0);
}