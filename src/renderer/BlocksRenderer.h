//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_BLOCKSRENDERER_H
#define SHITCRAFT_BLOCKSRENDERER_H

#include "core/Drawable.h"
#include "ChunkMesh.h"
#include "world/World.h"
#include "WrapAround3dArray.h"

class BlocksRenderer : public Drawable {
public:
    explicit BlocksRenderer(World &world);

    ~BlocksRenderer() override;

    void draw(const Transform &transform) const final;

private:
    static Texture createArrayTexture();

    void reloadChunks(AAB loadedRegion);

    void chunksStateChanged(AAB region, ChunkState newState);

    void updateChunkMesh(glm::ivec3 position, const Chunk &chunk);

    std::size_t id;

    WorldEventListener<ChunksStateChanged> chunksStateChangedListener;
    WorldEventListener<ReloadChunks> reloadChunksListener;

    World &world;

    const Shader &shader;
    WrapAround3dArray<ChunkMesh> chunkMeshes;

    Texture texture;
};


#endif //SHITCRAFT_BLOCKSRENDERER_H
