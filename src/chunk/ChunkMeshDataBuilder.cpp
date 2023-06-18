//
// Created by egorv on 5/17/2023.
//

#include "ChunkMeshDataBuilder.h"

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

    for (std::size_t i = 0; i < 6; ++i)
        meshData.vertexCount += static_cast<int>(meshData.vertices[i].size());

    return std::move(meshData);
}

void ChunkMeshDataBuilder::build2dMesh(int originBlockOffset, int face) {
    const auto normalAxis = Axis::getPositiveDirection(Face::getNormalAxis(face));

    const auto originBlock = Face::getOriginNormalized(face) * (CHUNK_SIDE_BLOCK_COUNT - 1) +
                             normalAxis * originBlockOffset;

    auto originBlockWorld = glm::vec3(Face::getOriginNormalized(face) * CHUNK_SIDE_BLOCK_COUNT);
    originBlockWorld += normalAxis * originBlockOffset;

    if (isForwardOriented(face))
        originBlockWorld += glm::vec3(normalAxis);

    originBlockWorld *= BLOCK_SIDE_SCALE_F;

    const auto right = Face::getRightDirection(face);
    const auto top = Face::getTopDirection(face);

    std::vector<bool> visited(CHUNK_SIDE_BLOCK_COUNT * CHUNK_SIDE_BLOCK_COUNT, false);

    for (int y = 0; y < CHUNK_SIDE_BLOCK_COUNT; ++y) {
        for (int x = 0; x < CHUNK_SIDE_BLOCK_COUNT; ++x) {
            const auto curBlock = originBlock + right * x + top * y;
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

            glm::vec3 curBlockWorld = originBlockWorld;

            curBlockWorld += glm::vec3(right * x) * BLOCK_SIDE_SCALE_F;
            curBlockWorld += glm::vec3(top * y) * BLOCK_SIDE_SCALE_F;

            auto rightWorld = glm::vec3(right) * BLOCK_SIDE_SCALE_F;
            auto topWorld = glm::vec3(top) * BLOCK_SIDE_SCALE_F;

            constexpr float TEXTURE_PADDING = 0.f;

            float minU = TEXTURE_PADDING, minV = TEXTURE_PADDING;

            auto maxU = static_cast<float>(rx - x + 1) - TEXTURE_PADDING;
            auto maxV = static_cast<float>(ty - y + 1) - TEXTURE_PADDING;

            glm::vec3 quadVertexCoords[4] = {
                    curBlockWorld,
                    curBlockWorld + rightWorld * maxU,
                    curBlockWorld + rightWorld * maxU + topWorld * maxV,
                    curBlockWorld + topWorld * maxV
            };

            glm::vec3 color(1.f);
            color *= brightnessCoefficient[face];

            Vertex quadVertices[4] = {
                    Vertex(quadVertexCoords[0], glm::vec2(minU, minV), color, blockFace.textureId),
                    Vertex(quadVertexCoords[1], glm::vec2(maxU, minV), color, blockFace.textureId),
                    Vertex(quadVertexCoords[2], glm::vec2(maxU, maxV), color, blockFace.textureId),
                    Vertex(quadVertexCoords[3], glm::vec2(minU, maxV), color, blockFace.textureId)
            };

            meshData.vertices[face].push_back(quadVertices[0]);
            meshData.vertices[face].push_back(quadVertices[1]);
            meshData.vertices[face].push_back(quadVertices[2]);
            meshData.vertices[face].push_back(quadVertices[0]);
            meshData.vertices[face].push_back(quadVertices[2]);
            meshData.vertices[face].push_back(quadVertices[3]);

            for (int i = y; i <= ty; ++i) {
                for (int j = x; j <= rx; ++j) {
                    int pos = i * CHUNK_SIDE_BLOCK_COUNT + j;
                    visited[pos] = true;
                }
            }
        }
    }
}