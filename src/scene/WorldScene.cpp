//
// Created by egorv on 4/26/2023.
//

#include "renderer/BlocksRenderer.h"
#include "WorldScene.h"

WorldScene::WorldScene() {
    addDrawable(std::make_shared<BlocksRenderer>());
}
