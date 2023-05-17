//
// Created by egorv on 4/26/2023.
//

#include "renderer/BlocksRenderer.h"
#include "camera/CameraControls.h"
#include "WorldScene.h"

constexpr double P_FOV_RAD = glm::pi<double>() / 3.0;
constexpr double P_NEAR = 0.001;
constexpr double P_FAR = 200.0;

constexpr float P_FOV_RAD_F = static_cast<float>(P_FOV_RAD);
constexpr float P_NEAR_F = static_cast<float>(P_NEAR);
constexpr float P_FAR_F = static_cast<float>(P_FAR);

WorldScene::WorldScene() {
    auto cameraControls = std::make_shared<CameraControls>(camera);

    scheduleForUpdates(cameraControls);
    addEventConsumer(cameraControls);

    addDrawable(std::make_shared<BlocksRenderer>(world));
    world.reloadAllChunks(AAB(glm::ivec3(0), glm::ivec3(9, 0, 9)));
}

void WorldScene::onUpdate(uint64_t deltaMs) {
    Perspective perspective{winAspectRatio, P_FOV_RAD, P_NEAR, P_FAR};
    ViewFrustrum frustrum(perspective, camera.getPosition(), camera.getFront(), camera.getRight());

    setTransform({projMat * camera.getViewMatrix(), frustrum});
}

bool WorldScene::onHandleEvent(const SDL_Event &event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        auto winWidth = static_cast<double>(event.window.data1), winHeight = static_cast<double>(event.window.data2);
        winAspectRatio = winWidth / winHeight;

        projMat = glm::perspective(P_FOV_RAD_F, static_cast<float>(winAspectRatio), P_NEAR_F, P_FAR_F);
    }

    return false;
}