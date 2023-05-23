#version 330 core

in vec3 vViewPos;
in vec3 vTexCoords;
in vec3 vColor;
in vec3 vNormal;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;
//uniform mat4 viewProjection;

vec2 uv_cstantos( vec2 uv, vec2 res ) {
    vec2 pixels = uv * res;

    // Updated to the final article
    vec2 alpha = 0.7 * fwidth(pixels);
    vec2 pixels_fract = fract(pixels);
    vec2 pixels_diff = clamp( .5 / alpha * pixels_fract, 0.0, .5 ) + clamp( .5 / alpha * (pixels_fract - 1) + .5, 0.0, .5 );
    pixels = floor(pixels) + pixels_diff;
    return pixels / res;
}

vec2 uv_aa_linear( vec2 uv, vec2 res, float width )
{
    uv = uv * res;
    vec2 uv_floor = floor(uv + 0.5);
    uv = uv_floor + clamp( (uv - uv_floor) / fwidth(uv) / width, -0.5, 0.5);
    return uv / res;
}

float getLod() {
    vec2 size = vec2(textureSize(arrayTexture, 0).xy);

    vec2 dx = dFdx(vTexCoords.xy * size);
    vec2 dy = dFdy(vTexCoords.xy * size);

    float d = max(length(dx), length(dy));
    float lod = max(0., log2(d));

    return lod;
}

void main() {
    ivec2 textureSize = textureSize(arrayTexture, 0).xy;

    float lod = getLod();
    ivec2 lodTextureSize = textureSize(arrayTexture, int(lod)).xy;

    vec3 texCoords = vec3(uv_aa_linear(vTexCoords.xy, lodTextureSize, 0.7), vTexCoords.z);

    vec3 color = texture(arrayTexture, texCoords).rgb * vColor;
    FragColor = vec4((color - 0.5) * 1.1 + 0.5, 1.0);
}