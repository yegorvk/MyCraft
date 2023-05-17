#version 330 core

in vec3 vViewPos;
in vec3 vTexCoords;
in vec3 vColor;
in vec3 vNormal;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;
//uniform mat4 viewProjection;

void main() {
    //vec3 lightDir = normalize(viewProjection * vec3(0.0, -1.0, 0.0));
    FragColor = vec4(texture(arrayTexture, vTexCoords).rgb * vColor, 1.0);
}