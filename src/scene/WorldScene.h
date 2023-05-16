//
// Created by egorv on 4/26/2023.
//

#ifndef SHITCRAFT_WORLDSCENE_H
#define SHITCRAFT_WORLDSCENE_H

#include "core/Node.h"
#include "world/World.h"

class WorldScene : public Node {
public:
    WorldScene();

    ~WorldScene() override = default;

private:
    World world;
};


#endif //SHITCRAFT_WORLDSCENE_H
