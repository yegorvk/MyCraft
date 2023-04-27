#version 330 core

in vec3 vTexCoords;
in vec3 vColor;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

void main() {
    FragColor = vec4(texture(arrayTexture, vTexCoords).rgb * vColor, 1.0);
}