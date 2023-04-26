#version 330 core

in vec2 vUv;

out vec4 FragColor;

uniform sampler2DArray tex;

void main() {
    FragColor = vec4(texture(tex, vec3(vUv, 0.0)).rgb, 1);
}