#version 330 core

in vec2 vUv;

out vec4 FragColor;

uniform sampler2DArray tex;

void main() {
    vec3 texColor = texture(tex, vec3(vUv, 0.0)).rgb;
    FragColor = vec4(texColor * texColor * texColor * texColor, 1);
}