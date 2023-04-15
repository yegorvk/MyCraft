//
// Created by egorv on 4/8/2023.
//

#include "core/NodeHost.h"

void NodeHost::draw(const Transform &transform) const {
    if (currentNode)
        currentNode->draw(transform);
}

bool NodeHost::handleEvent(const SDL_Event &event) {
    return currentNode && currentNode->handleEvent(event);
}

void NodeHost::update(uint64_t deltaMs) {
    if (currentNode)
        currentNode->update(deltaMs);
}
