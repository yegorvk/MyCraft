//
// Created by egorv on 4/26/2023.
//

#include "renderer/ChunkRenderer.h"
#include "camera/CameraControls.h"
#include "WorldScene.h"
#include "GameContext.h"

constexpr double P_FOV_RAD = glm::pi<double>() / 3.0;
constexpr double P_NEAR = 0.1;
constexpr double P_FAR = 2000.0;

WorldScene::WorldScene() {
    auto cameraControls = std::make_shared<CameraControls>(camera);

    scheduleForUpdates(cameraControls);
    addEventConsumer(cameraControls);

    world.setActiveRegion(glm::ivec3(0, 0, 0), glm::ivec3(32, 10, 32));
    renderer.resetActiveChunks(world.getActiveRegionMin(), world.getActiveRegionSize());

    camera.moveAbsolute(
            glm::dvec3(2 * world.getActiveRegionMin() + world.getActiveRegionSize() - 1) / 2.0 * CHUNK_SIDE_SCALE);

    world.setPlayerPosition(camera.getPosition() / BLOCK_SIDE_SCALE);
}

void WorldScene::onPreDraw() {
    Perspective perspective{winAspectRatio, P_FOV_RAD, P_NEAR, P_FAR};
    ViewFrustrum frustrum(perspective, glm::vec3(0), camera.getFront(), camera.getRight());

    const auto view = camera.getViewMatrix();

    RenderState state{perspective, frustrum, view, camera.getPosition()};
    renderer.draw(state);
}

void WorldScene::onUpdate(uint64_t deltaMs) {
    world.processPlayerPositionChange(camera.getPosition() / BLOCK_SIDE_SCALE);
    world.dispatchChunkLoads();

    renderer.setActiveChunksMin(world.getActiveRegionMin());

    while (true) {
        auto meshUpdateReq = world.dequeueMeshUpdateRequest();

        if (meshUpdateReq.has_value()) {
            auto req = meshUpdateReq.value();
            renderer.updateChunk(req.position, req.chunk ? &req.chunk->getMeshData() : nullptr);
        } else
            break;
    }
}

bool WorldScene::onHandleEvent(const SDL_Event &event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        auto winWidth = static_cast<double>(event.window.data1), winHeight = static_cast<double>(event.window.data2);
        winAspectRatio = winWidth / winHeight;

        viewportSize = glm::vec2(winWidth, winHeight);
        renderer.resize(viewportSize);
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT)
            world.setTargetBlockAndQueueUpdate(camera.getPosition() / BLOCK_SIDE_SCALE, camera.getFront(), 0);
        else if (event.button.button == SDL_BUTTON_RIGHT)
            world.setTargetBlockAndQueueUpdate(camera.getPosition() / BLOCK_SIDE_SCALE, camera.getFront(), 3);
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS) {
            ++renderDistance;
        } else if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) {
            if (renderDistance > 1)
                --renderDistance;
        }
    }

    return false;
}