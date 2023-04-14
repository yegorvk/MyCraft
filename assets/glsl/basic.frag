#version 330 core

in vec2 vUv;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = vec4(texture(tex, vUv).rgb, 1.0);
}