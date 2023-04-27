//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_SIMPLECUBESCENE_H
#define SHITCRAFT_SIMPLECUBESCENE_H

#include "core/Node.h"
#include "SimpleCube.h"

struct SimpleCubeScene : public Node {
    inline SimpleCubeScene() : Node() {
        addDrawable(std::make_shared<SimpleCube>());
    }
};

#endif //SHITCRAFT_SIMPLECUBESCENE_H
