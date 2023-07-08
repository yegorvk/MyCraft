//
// Created by egorv on 4/8/2023.
//

#include "Node.h"
#include <algorithm>

void Node::draw() {
    onPreDraw();

    for (const auto& drawable : drawables)
        drawable.child->draw();

    onPostDraw();
}

bool Node::handleEvent(const SDL_Event &event) {
    if (onHandleEvent(event))
        return true;

    for (const auto& consumer : eventConsumers) {
        if (consumer.child->handleEvent(event))
            return true;
    }

    return false;
}

void Node::update(uint64_t deltaMs) {
    onUpdate(deltaMs);

    for (const auto& delegate : updateDelegates)
        delegate.child->update(deltaMs);
}

void Node::onPreDraw()  {}

void Node::onPostDraw() {}

bool Node::onHandleEvent(const SDL_Event &event) {
    return false;
}

void Node::onUpdate(uint64_t deltaMs) {}