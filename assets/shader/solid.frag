#version 400 core

in VS_OUT {
    vec2 texCoords;
    flat uint textureId;
    vec3 color;
} fsIn;

out vec4 FragColor;

uniform sampler2DArray arrayTexture;

vec2 uv_nearest(vec2 uv, ivec2 texture_size) {
    vec2 pixel = uv * texture_size;
    pixel = floor(pixel) + .5;

    return pixel / texture_size;
}

float mip_map_level(in vec2 texture_coordinate) {
    vec2 dx_vtc = dFdx(texture_coordinate);
    vec2 dy_vtc = dFdy(texture_coordinate);
    float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
    return 0.5 * log2(delta_max_sqr);
}

vec2 uv_aa_smoothstep(vec2 uv, vec2 res, float width) {
    uv = uv * res;
    vec2 uv_floor = floor(uv + 0.5);
    vec2 uv_fract = fract(uv + 0.5);
    vec2 uv_aa = fwidth(uv) * width * 0.5;
    uv_fract = smoothstep(
        vec2(0.5) - uv_aa,
        vec2(0.5) + uv_aa,
        uv_fract
    );

    return (uv_floor + uv_fract - 0.5) / res;
}

vec2 uv_aa(vec2 uv, vec2 res) {
    uv = uv*res + 0.5;

    // tweak fractionnal value of the registry coordinate
    vec2 fl = floor(uv);
    vec2 fr = fract(uv);
    vec2 aa = fwidth(uv)*0.75;
    fr = smoothstep( vec2(0.5)-aa, vec2(0.5)+aa, fr);

    return (fl+fr-0.5) / res;
}

vec4 texture2DArrayAA(sampler2DArray tex, vec2 uv, float id) {
    float lod = textureQueryLod(tex, uv).x;
    vec2 texsize = vec2(mix(vec2(textureSize(tex, int(floor(lod)))), vec2(textureSize(tex, int(ceil(lod)))), fract(lod)).xy);
    vec2 uv_texspace = uv * texsize;
    vec2 seam = floor(uv_texspace + .5);
    uv_texspace = (uv_texspace - seam) / fwidth(uv_texspace) + seam;
    uv_texspace = clamp(uv_texspace, seam - .5, seam + .5);

    return textureLod(tex, vec3(uv_texspace / texsize, id), lod);
}

vec4 sampleTexture(sampler2DArray tex, vec2 uv, float id) {
    float lod = round(textureQueryLod(tex, uv).x);
    vec2 res = vec2(textureSize(tex, int(lod)));
    vec2 transformedUv = uv_aa(uv, res);

    return textureLod(tex, vec3(transformedUv, id), lod);
}

void main() {
    //    float level = mip_map_level(fsIn.texCoords);
    //    vec2 textureSize = textureSize(arrayTexture, int(round(level))).xy;
    //    //    vec2 uv = uv_nearest(fsIn.texCoords, ivec2(textureSize));
    vec3 textureColor = texture2DArrayAA(arrayTexture, vec2(fsIn.texCoords), float(fsIn.textureId - 1u)).rgb;
    //vec3 textureColor = sampleTexture(arrayTexture, vec2(fsIn.texCoords), float(fsIn.textureId - 1u)).rgb;
    //vec3 textureColor = registry(arrayTexture, vec3(fsIn.texCoords, float(fsIn.textureId - 1u))).rgb;
    vec3 color = textureColor * fsIn.color;

    FragColor = vec4(color, 1.0);
}