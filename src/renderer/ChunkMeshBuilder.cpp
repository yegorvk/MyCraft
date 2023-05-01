//
// Created by egorv on 4/29/2023.
//

#include "ChunkMeshBuilder.h"

constexpr glm::ivec3 FACE_RIGHT[6] = {
        {0,  0, -1},
        {0,  0, 1},
        {1,  0, 0},
        {1,  0, 0},
        {1,  0, 0},
        {-1, 0, 0}
};

constexpr glm::ivec3 FACE_TOP[6] = {
        {0, 1, 0},
        {0, 1, 0},
        {0, 0, -1},
        {0, 0, 1},
        {0, 1, 0},
        {0, 1, 0}
};

constexpr glm::ivec3 FACE_ORIGIN[6] = {
        {0, 0, 1},
        {0, 0, 0},
        {0, 0, 1},
        {0, 0, 0},
        {0, 0, 0},
        {1, 0, 0}
};

constexpr glm::ivec3 AXIS[3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
};

ChunkMeshBuilder::ChunkMeshBuilder(const Chunk &chunk, const BlockCache &blockCache, glm::vec3 offset,
                                   float blockSideLen)
        : chunk(chunk), blockCache(blockCache), offset(offset), blockSideLen(blockSideLen) {
    std::vector<bool> blockFaceOrientation(chunk.getFaceBlockCount());

    for (int i = 0; i < 3; ++i) {
        auto axis = static_cast<Axis>(i);

        std::fill(blockFaceOrientation.begin(), blockFaceOrientation.end(), true);

        for (int j = 0; j < chunk.getSideBlockCount(); ++j) {
            build2dMesh(j, face(axis, true), axis, blockFaceOrientation);
            build2dMesh(j, face(axis, false), axis, blockFaceOrientation);
        }
    }
}

int ChunkMeshBuilder::getTotalVertexCount() const {
    int count = 0;

    for (const auto &subMeshVertices: vertices)
        count += static_cast<int>(subMeshVertices.size());

    return count;
}

void ChunkMeshBuilder::build2dMesh(int originBlockOffset, BlockFace face, Axis axis,
                                   std::vector<bool> &blockFaceOrientation) {
    auto axisIndex = static_cast<int>(axis);
    auto faceIndex = static_cast<int>(face);

    auto originBlock = FACE_ORIGIN[faceIndex] * (chunk.getSideBlockCount() - 1) + AXIS[axisIndex] * originBlockOffset;

    auto originBlockWorld = glm::vec3(FACE_ORIGIN[faceIndex] * chunk.getSideBlockCount());
    originBlockWorld += AXIS[axisIndex] * originBlockOffset;

    if (faceOrientationAlongNormalAxis(face))
        originBlockWorld += glm::vec3(AXIS[axisIndex]);

    originBlockWorld *= blockSideLen;

    auto right = FACE_RIGHT[faceIndex];
    auto top = FACE_TOP[faceIndex];

    std::vector<bool> visited(chunk.getFaceBlockCount(), false);

    for (int y = 0; y < chunk.getSideBlockCount(); ++y) {
        for (int x = 0; x < chunk.getSideBlockCount(); ++x) {
            auto curBlock = originBlock + right * x + top * y;
            auto texId = getTexId(curBlock, face);

            if (visited[(y << chunk.getSideBlockCountLog2()) + x] || !hasTranslucentNeighbour(curBlock, faceIndex) ||
                texId == 0)
                continue;

            int rx = x, ty = y;

            while (rx < chunk.getSideBlockCount() - 1) {
                auto nextPos = curBlock + right * (rx - x + 1);

                if (visited[(y << chunk.getSideBlockCountLog2()) + (rx + 1)] ||
                    !hasTranslucentNeighbour(nextPos, faceIndex) || getTexId(nextPos, face) != texId)
                    break;
                else
                    ++rx;
            }

            while (ty < chunk.getSideBlockCount() - 1) {
                auto nextRowLeft = curBlock + top * (ty - y + 1);
                bool f = true;

                for (int i = x; i <= rx; ++i) {
                    auto coords = nextRowLeft + right * (i - x);

                    if (visited[((ty + 1) << chunk.getSideBlockCountLog2()) + i] ||
                        !hasTranslucentNeighbour(coords, faceIndex) || getTexId(coords, face) != texId) {
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

            curBlockWorld += glm::vec3(right * x) * blockSideLen;
            curBlockWorld += glm::vec3(top * y) * blockSideLen;

            auto rightWorld = glm::vec3(right) * blockSideLen;
            auto topWorld = glm::vec3(top) * blockSideLen;

            auto maxU = static_cast<float>(rx - x + 1);
            auto maxV = static_cast<float>(ty - y + 1);

            glm::vec3 quadVertexCoords[4] = {
                    curBlockWorld,
                    curBlockWorld + rightWorld * maxU,
                    curBlockWorld + rightWorld * maxU + topWorld * maxV,
                    curBlockWorld + topWorld * maxV
            };

            glm::vec3 color(1.f);

            if (texId == 1)
                color = glm::vec3(0.f, 1.f, 0.f);

            Vertex quadVertices[4] = {
                    {quadVertexCoords[0], glm::vec3(0.f, 0.f, texId), color},
                    {quadVertexCoords[1], glm::vec3(maxU, 0.f, texId), color},
                    {quadVertexCoords[2], glm::vec3(maxU, maxV, texId), color},
                    {quadVertexCoords[3], glm::vec3(0.f, maxV, texId), color}
            };

            vertices[faceIndex].push_back(quadVertices[0]);
            vertices[faceIndex].push_back(quadVertices[1]);
            vertices[faceIndex].push_back(quadVertices[2]);
            vertices[faceIndex].push_back(quadVertices[0]);
            vertices[faceIndex].push_back(quadVertices[2]);
            vertices[faceIndex].push_back(quadVertices[3]);

            for (int i = y; i <= ty; ++i) {
                for (int j = x; j <= rx; ++j) {
                    int pos = (i << chunk.getSideBlockCountLog2()) + j;

                    visited[pos] = true;
                    blockFaceOrientation[pos] = !blockFaceOrientation[pos];
                }
            }
        }
    }
}