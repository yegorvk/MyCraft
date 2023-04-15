//
// Created by egorv on 4/8/2023.
//

#include "core/Node.h"
#include <algorithm>

void Node::draw(const Transform &transform) const {
    for (const auto& drawable : drawables)
        drawable.child->draw(transform);
}

bool Node::handleEvent(const SDL_Event &event) {
    for (const auto& consumer : eventConsumers) {
        if (consumer.child->handleEvent(event))
            return true;
    }

    return false;
}

void Node::update(uint64_t deltaMs) {
    for (const auto& delegate : updateDelegates)
        delegate.child->update(deltaMs);
}

void Node::onDraw(const glm::mat4 &transform) {}

bool Node::onHandleEvent(const SDL_Event &event) {
    return false;
}

void Node::onUpdate(uint64_t deltaMs) {}