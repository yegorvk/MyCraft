//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_NODEHOST_H
#define SHITCRAFT_NODEHOST_H

#include "core/INode.h"
#include <memory>

class NodeHost : public INode {
public:
    NodeHost() = default;

    template <typename T>
    inline explicit NodeHost(T&& node) : currentNode(std::make_unique<T>(node)) {}

    template <typename T>
    inline void setNode(T&& node) {
        currentNode = std::make_unique<T>(node);
    }

    void draw(const Transform &transform) const override;

    bool handleEvent(const SDL_Event &event) override;

    void update(uint64_t deltaMs) override;
private:
    std::unique_ptr<INode> currentNode;
};

#endif //SHITCRAFT_NODEHOST_H
