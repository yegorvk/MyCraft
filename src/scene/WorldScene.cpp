//
// Created by egorv on 4/26/2023.
//

#include "renderer/BlocksRenderer.h"
#include "WorldScene.h"

constexpr double P_FOV_RAD = glm::pi<double>() / 3.0;
constexpr double P_NEAR = 0.001;
constexpr double P_FAR = 200.0;

constexpr float P_FOV_RAD_F = static_cast<float>(P_FOV_RAD);
constexpr float P_NEAR_F = static_cast<float>(P_NEAR);
constexpr float P_FAR_F = static_cast<float>(P_FAR);

WorldScene::WorldScene() {
    addDrawable(std::make_shared<BlocksRenderer>(world));
    world.reloadedAllChunks(AAB(glm::ivec3(0), glm::ivec3(9, 0, 9)));
}
