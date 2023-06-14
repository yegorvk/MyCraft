//
// Created by egorv on 5/17/2023.
//

#include "ChunkData.h"
#include "ChunkMeshDataBuilder.h"

ChunkData::ChunkData() {
    blocks.resize(CHUNK_SIDE_BLOCK_COUNT*CHUNK_SIDE_BLOCK_COUNT*CHUNK_SIDE_BLOCK_COUNT, 0);
}

void ChunkData::updateMesh(const BlockRegistry &blockCache) {
    meshData = ChunkMeshDataBuilder(blocks.data(), blockCache).build();
}
