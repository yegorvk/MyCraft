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

constexpr float brightnessCoefficient[6] = {
        .8f,
        .8f,
        1.f,
        .1f,
        .7f,
        .7f
};

ChunkMeshBuilder::ChunkMeshBuilder(const Chunk &chunk, const BlockCache &blockCache, glm::vec3 offset,
                                   float blockSideLen)
        : chunk(chunk), blockCache(blockCache), offset(offset), blockSideLen(blockSideLen) {
    for (int axis = 0; axis < 3; ++axis) {
        for (int j = 0; j < chunk.getSideBlockCount(); ++j) {
            build2dMesh(j, face(axis, false), axis);
            build2dMesh(j, face(axis, true), axis);
        }
    }
}

int ChunkMeshBuilder::getTotalVertexCount() const {
    int count = 0;

    for (const auto &subMeshVertices: vertices)
        count += static_cast<int>(subMeshVertices.size());

    return count;
}

void ChunkMeshBuilder::build2dMesh(int originBlockOffset, int face, int normalAxis) {
    auto originBlock = FACE_ORIGIN[face] * (chunk.getSideBlockCount() - 1) + AXIS[normalAxis] * originBlockOffset;

    auto originBlockWorld = glm::vec3(FACE_ORIGIN[face] * chunk.getSideBlockCount());
    originBlockWorld += AXIS[normalAxis] * originBlockOffset;

    if (isForwardOriented(face))
        originBlockWorld += glm::vec3(AXIS[normalAxis]);

    originBlockWorld = offset + originBlockWorld * blockSideLen;

    auto right = FACE_RIGHT[face];
    auto top = FACE_TOP[face];

    std::vector<bool> visited(chunk.getFaceBlockCount(), false);

    for (int y = 0; y < chunk.getSideBlockCount(); ++y) {
        for (int x = 0; x < chunk.getSideBlockCount(); ++x) {
            auto curBlock = originBlock + right * x + top * y;
            auto texId = getTexId(curBlock, face);

            if (visited[(y << chunk.getSideBlockCountLog2()) + x] || !hasTranslucentNeighbour(curBlock, face) ||
                texId == 0)
                continue;

            int rx = x, ty = y;

            while (rx < chunk.getSideBlockCount() - 1) {
                auto nextPos = curBlock + right * (rx - x + 1);

                if (visited[(y << chunk.getSideBlockCountLog2()) + (rx + 1)] ||
                    !hasTranslucentNeighbour(nextPos, face) || getTexId(nextPos, face) != texId)
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
                        !hasTranslucentNeighbour(coords, face) || getTexId(coords, face) != texId) {
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

            color *= brightnessCoefficient[face];

            glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(right), glm::vec3(top)));

            if (isForwardOriented(face))
                normal = -normal;

            Vertex quadVertices[4] = {
                    {quadVertexCoords[0], glm::vec3(0.f, 0.f, texId), normal, color},
                    {quadVertexCoords[1], glm::vec3(maxU, 0.f, texId), normal, color},
                    {quadVertexCoords[2], glm::vec3(maxU, maxV, texId), normal, color},
                    {quadVertexCoords[3], glm::vec3(0.f, maxV, texId), normal, color}
            };

            vertices[face].push_back(quadVertices[0]);
            vertices[face].push_back(quadVertices[1]);
            vertices[face].push_back(quadVertices[2]);
            vertices[face].push_back(quadVertices[0]);
            vertices[face].push_back(quadVertices[2]);
            vertices[face].push_back(quadVertices[3]);

            for (int i = y; i <= ty; ++i) {
                for (int j = x; j <= rx; ++j) {
                    int pos = (i << chunk.getSideBlockCountLog2()) + j;
                    visited[pos] = true;
                }
            }
        }
    }
}