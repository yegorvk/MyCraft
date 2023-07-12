#version 330 core

const uint MASK_ID = 0xFFFu;
const uint MASK_X = 0x1Fu;
const uint MASK_Y = 0x1Fu;
const uint MASK_Z = 0x1Fu;
const uint MASK_AO = 0x3u;

const uint SHIFT_ID = 17u;
const uint SHIFT_X = 12u;
const uint SHIFT_Y = 7u;
const uint SHIFT_Z = 2u;
const uint SHIFT_AO = 0u;

const uint MASK_U = 0x1Fu;
const uint MASK_V = 0x1Fu;

const uint SHIFT_U = 5u;
const uint SHIFT_V = 0u;

struct Vertex {
    uvec3 position;
    uvec2 uv;
    uint ao;
    uint textureId;
};

Vertex unpack(uvec2 packedVertex) {
    Vertex v;

    v.position.x = (packedVertex.x >> SHIFT_X) & MASK_X;
    v.position.y = (packedVertex.x >> SHIFT_Y) & MASK_Y;
    v.position.z = (packedVertex.x >> SHIFT_Z) & MASK_Z;

    v.ao = (packedVertex.x >> SHIFT_AO) & MASK_AO;

    v.uv.s = (packedVertex.y >> SHIFT_U) & MASK_U;
    v.uv.t = (packedVertex.y >> SHIFT_V) & MASK_V;

    v.textureId = (packedVertex.x >> SHIFT_ID) & MASK_ID;

    return v;
}

layout (location = 0) in uvec2 v;

uniform mat4 mvp;
uniform float blockScale;

out VS_OUT {
    vec2 texCoords;
    flat uint textureId;
    vec3 color;
} vsOut;

const vec3 AO_CURVE[4] = vec3[](vec3(0.6), vec3(0.75), vec3(0.9), vec3(1.0));

const float PAD = 0.003;
const vec2 TEX_PAD[6] = vec2[](vec2(PAD), vec2(-PAD, PAD), vec2(-PAD), vec2(PAD), vec2(-PAD), vec2(PAD, -PAD));

void main() {
    Vertex vertex = unpack(v);

    vsOut.texCoords = vec2(vertex.uv) + TEX_PAD[gl_VertexID % 6];
    vsOut.textureId = vertex.textureId;
    vsOut.color = AO_CURVE[vertex.ao];

    vec3 positionWorld = blockScale * vec3(vertex.position);
    vec4 position = mvp * vec4(positionWorld, 1.0);

    gl_Position = position;
}