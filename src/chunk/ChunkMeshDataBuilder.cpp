//
// Created by egorv on 5/17/2023.
//

#include <iostream>

#include "ChunkMeshDataBuilder.h"
#include "ChunkVertex.h"

constexpr float brightnessCoefficient[6] = {
        .8f,
        .8f,
        1.f,
        .1f,
        .7f,
        .7f
};

ChunkMeshDataBuilder::ChunkMeshDataBuilder(const ChunkBlocks &blocks, const BlockRegistry &blockCache)
        : blocks(blocks), blockCache(blockCache) {}

ChunkMeshData ChunkMeshDataBuilder::build() {
    for (int axis = 0; axis < 3; ++axis) {
        for (int j = 0; j < CHUNK_SIDE_BLOCK_COUNT; ++j) {
            build2dMesh(j, Face::fromAxis(axis, false));
            build2dMesh(j, Face::fromAxis(axis, true));
        }
    }

    return std::move(meshData);
}

void ChunkMeshDataBuilder::build2dMesh(int originBlockOffset, int face) {
    const auto normalAxis = Axis::getPositiveDirection(Face::getNormalAxis(face));

    const glm::ivec3 originBlock = Face::getOriginNormalized(face) * (CHUNK_SIDE_BLOCK_COUNT - 1) +
                             normalAxis * originBlockOffset;

    const auto normal = Face::getNormalDirection(face);
    const auto right = Face::getRightDirection(face);
    const auto top = Face::getTopDirection(face);

    const auto rightAxis = Face::getRightAxis(face);
    const auto topAxis = Face::getTopAxis(face);

    glm::ivec3 originVertexCoordsOffset{};

    if (face % 2 == 0)
        originVertexCoordsOffset += normal;

    if (right[rightAxis] < 0)
        originVertexCoordsOffset -= right;

    if (top[topAxis] < 0)
        originVertexCoordsOffset -= top;

    std::vector<bool> visited(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT, false);

    for (int y = 0; y < CHUNK_SIDE_BLOCK_COUNT; ++y) {
        for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x) {
            const glm::ivec3 curBlock = originBlock + right * x + top * y;
            const auto blockFace = getFace(curBlock, face);

            if (visited[y * CHUNK_SIDE_BLOCK_COUNT + x] || !hasTranslucentNeighbour(curBlock, face) ||
                blockFace.textureId == 0)
                continue;

            int rx = x, ty = y;

            while (rx < CHUNK_SIDE_BLOCK_COUNT - 1) {
                auto nextPos = curBlock + right * (rx - x + 1);

                if (visited[y * CHUNK_SIDE_BLOCK_COUNT + (rx + 1)] ||
                    !hasTranslucentNeighbour(nextPos, face) || getFace(nextPos, face) != blockFace)
                    break;
                else
                    ++rx;
            }

            while (ty < CHUNK_SIDE_BLOCK_COUNT - 1) {
                auto nextRowLeft = curBlock + top * (ty - y + 1);
                bool f = true;

                for (int i = x; i <= rx; ++i) {
                    auto coords = nextRowLeft + right * (i - x);

                    if (visited[(ty + 1) * CHUNK_SIDE_BLOCK_COUNT + i] || !hasTranslucentNeighbour(coords, face) ||
                        getFace(coords, face) != blockFace) {
                        f = false;
                        break;
                    }
                }

                if (!f)
                    break;
                else
                    ++ty;
            }

            const int minU = 0, minV = 0;

            const int maxU = rx - x + 1;
            const int maxV = ty - y + 1;

            auto originVertexCoords = curBlock + originVertexCoordsOffset;

            const glm::ivec3 quadVertexCoords[4] = {
                    originVertexCoords,
                    originVertexCoords + right * maxU,
                    originVertexCoords + right * maxU + top * maxV,
                    originVertexCoords + top * maxV
            };

            constexpr int AO[6] {
                2,
                1,
                3,
                2,
                2,
                1
            };

            const int ao = AO[face];

            PackedChunkVertex quadVertices[4] = {
                    {quadVertexCoords[0], glm::uvec2(minU, minV), face, blockFace.textureId, ao},
                    {quadVertexCoords[1], glm::uvec2(maxU, minV), face, blockFace.textureId, ao},
                    {quadVertexCoords[2], glm::uvec2(maxU, maxV), face, blockFace.textureId, ao},
                    {quadVertexCoords[3], glm::uvec2(minU, maxV), face, blockFace.textureId, ao}
            };

            meshData.vertices.push_back(quadVertices[0]);
            meshData.vertices.push_back(quadVertices[1]);
            meshData.vertices.push_back(quadVertices[2]);
            meshData.vertices.push_back(quadVertices[0]);
            meshData.vertices.push_back(quadVertices[2]);
            meshData.vertices.push_back(quadVertices[3]);

            for (int i = y; i <= ty; ++i) {
                for (int j = x; j <= rx; ++j) {
                    int pos = i * CHUNK_SIDE_BLOCK_COUNT + j;
                    visited[pos] = true;
                }
            }

            x = rx;
        }
    }
}

int ChunkMeshDataBuilder::getVertexAO(glm::ivec3 block, int face, int vertex) {
    auto off_1 = Face::getRightDirection(face);
    auto off_2 = Face::getTopDirection(face);

    if (vertex == 0 || vertex == 3)
        off_1 = -off_1;

    if (vertex == 0 || vertex == 1)
        off_2 = -off_2;

    bool side1 = blocks.getLocalChecked(block + off_1 + glm::ivec3(0, 1, 0)) != 0;
    bool side2 = blocks.getLocalChecked(block + off_2 + glm::ivec3(0, 1, 0)) != 0;
    bool corner = blocks.getLocalChecked(block + off_1 + off_2 + glm::ivec3(0, 1, 0)) != 0;

    return getVertexAO(side1, side2, corner);
}

bool ChunkMeshDataBuilder::hasEqualAO(glm::ivec3 block, int face, int ao) {
    for (int i = 0; i < 4; ++i) {
        if (getVertexAO(block, face, i) != ao)
            return false;
    }

    return true;
}