#version 330 core

in VS_OUT {
    vec2 texCoords;
    vec3 color;
    flat int textureId;
} fsIn;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

void main() {
    vec4 color = texture(arrayTexture, vec3(fsIn.texCoords, fsIn.textureId - 1));
    FragColor = vec4(color.rgb * fsIn.color, 1.0);
}