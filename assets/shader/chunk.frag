#version 330 core

in vec3 vTexCoords;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

void main() {
    FragColor = vec4(texture(arrayTexture, vTexCoords).rgb, 1.0);
}