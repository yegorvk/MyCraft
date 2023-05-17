//
// Created by egorv on 4/8/2023.
//

#include "Node.h"
#include <algorithm>

void Node::draw(const std::optional<Transform> &transform) const {
    const auto &curTransform = transform.has_value() ? transform : localTransform;

    onPreDraw(curTransform);

    for (const auto& drawable : drawables)
        drawable.child->draw(curTransform);

    onPostDraw(curTransform);
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

void Node::onPreDraw(const std::optional<Transform> &transform) const {}

void Node::onPostDraw(const std::optional<Transform> &transform) const {}

bool Node::onHandleEvent(const SDL_Event &event) {
    return false;
}

void Node::onUpdate(uint64_t deltaMs) {}