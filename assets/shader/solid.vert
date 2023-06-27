#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aColor;
layout (location = 3) in int aTextureId;

out VS_OUT {
    vec2 texCoords;
    vec3 color;
    flat int textureId;
} vsOut;

uniform mat4 mvp;

void main() {
    vsOut.texCoords = aTexCoords;
    vsOut.color = aColor;
    vsOut.textureId = aTextureId;

    vec4 position = mvp * vec4(aPosition, 1.0);

    gl_Position = position;

}