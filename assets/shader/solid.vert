#version 330 core

const uint VERT0_ID_MASK = 0xFFF00000u;
const uint VERT0_OFFSETS_MASK = 0xF0000u;
const uint VERT0_X_MASK = 0xF000u;
const uint VERT0_Y_MASK = 0xF00u;
const uint VERT0_Z_MASK = 0xF0u;
const uint VERT0_AO_MASK = 0xFu;

const uint VERT0_ID_SHIFT = 20u;
const uint VERT0_OFFSETS_SHIFT = 16u;
const uint VERT0_X_SHIFT = 12u;
const uint VERT0_Y_SHIFT = 8u;
const uint VERT0_Z_SHIFT = 4u;
const uint VERT0_AO_SHIFT = 0u;

const uint VERT1_U_MASK = 0x3E0u;
const uint VERT1_V_MASK = 0x1Fu;

const uint VERT1_U_SHIFT = 5u;
const uint VERT1_V_SHIFT = 0u;

struct Vertex {
    uvec3 position;
    uvec2 uv;
    uint ao;
    uint textureId;
};

Vertex unpack(uvec2 packedVertex) {
    Vertex v;

    v.position.x = (packedVertex.x & VERT0_X_MASK) >> VERT0_X_SHIFT;
    v.position.y = (packedVertex.x & VERT0_Y_MASK) >> VERT0_Y_SHIFT;
    v.position.z = (packedVertex.x & VERT0_Z_MASK) >> VERT0_Z_SHIFT;

    uint offses = (packedVertex.x & VERT0_OFFSETS_MASK) >> VERT0_OFFSETS_SHIFT;

    if ((offses & 0x4u) != 0u)
            ++v.position.x;

    if ((offses & 0x2u) != 0u)
            ++v.position.y;

    if ((offses & 0x1u) != 0u)
            ++v.position.z;

    v.ao = (packedVertex.x & VERT0_AO_MASK) >> VERT0_AO_SHIFT;

    v.uv.s = (packedVertex.y & VERT1_U_MASK) >> VERT1_U_SHIFT;
    v.uv.t = (packedVertex.y & VERT1_V_MASK) >> VERT1_V_SHIFT;

    v.textureId = (packedVertex.x & VERT0_ID_MASK) >> VERT0_ID_SHIFT;

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

const vec3 AO_CURVE[4] = vec3[](vec3(0.4), vec3(0.6), vec3(0.8), vec3(1.0));

void main() {
    Vertex vertex = unpack(v);

    vsOut.texCoords = vec2(vertex.uv);
    vsOut.textureId = vertex.textureId;
    vsOut.color = AO_CURVE[vertex.ao];

    vec4 position = mvp * vec4(blockScale * vec3(vertex.position), 1.0);
    gl_Position = position;
}