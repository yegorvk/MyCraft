#version 330 core

in vec3 vTexCoord;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

void main() {
    FragColor = vec4(texture(arrayTexture, vTexCoord).rgb, 1.0);
}