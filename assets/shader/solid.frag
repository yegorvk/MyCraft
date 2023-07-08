#version 330 core

in VS_OUT {
    vec2 texCoords;
    flat uint textureId;
    vec3 color;
} fsIn;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

float mip_map_level(in vec2 texture_coordinate) // in texel units
{
    vec2 dx_vtc = dFdx(texture_coordinate);
    vec2 dy_vtc = dFdy(texture_coordinate);
    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
    float mml = 0.5 * log2(delta_max_sqr);
    return max(0, mml); // Thanks @Nims
}

vec4 texture2DArrayAA(sampler2DArray tex, vec2 uv, float id) {
    float lod = mip_map_level(uv * textureSize(tex, 0).xy);

    vec2 texsize = vec2(mix(vec2(textureSize(tex, int(floor(lod)))), vec2(textureSize(tex, int(ceil(lod)))), fract(lod)).xy);
    vec2 uv_texspace = uv * texsize;
    vec2 seam = floor(uv_texspace + .5);
    uv_texspace = (uv_texspace - seam) / fwidth(uv_texspace) + seam;
    uv_texspace = clamp(uv_texspace, seam - .5, seam + .5);

    return textureLod(tex, vec3(uv_texspace / texsize, id), lod);
}

void main() {
    vec3 textureColor = texture2DArrayAA(arrayTexture, vec2(fsIn.texCoords), float(fsIn.textureId - 1u)).rgb;
    vec3 color = textureColor * fsIn.color;

    FragColor = vec4(color, 1.0);
}