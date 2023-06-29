//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_WORLDSCENE_H
#define SHITCRAFT_WORLDSCENE_H

#include "core/Node.h"
#include "world/World.h"
#include "camera/Camera.h"
#include "renderer/SolidRenderer.h"
#include "renderer/HUDRenderer.h"
#include "block/BlockRegistry.h"

class WorldScene : public Node {
public:
    WorldScene();

    void onPreDraw() const override;

    void onUpdate(uint64_t deltaMs) override;

    bool onHandleEvent(const SDL_Event &event) override;

    ~WorldScene() override = default;

private:
    Camera camera;

    double winAspectRatio = 1.0;
    glm::mat4 projMat{1.f};

    World world;

    SolidRenderer blocksRenderer;
    HUDRenderer hudRenderer;

    BlockRegistry blockReg;
};


#endif //SHITCRAFT_WORLDSCENE_H
