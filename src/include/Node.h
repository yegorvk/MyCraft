//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_NODE_H
#define SHITCRAFT_NODE_H

#include "INode.h"
#include <set>
#include <memory>

template<typename T>
struct ChildNodeEntry {
    inline explicit ChildNodeEntry(const std::shared_ptr<T> &child, int priority = 0) : child(child),
                                                                                        priority(priority) {}

    inline bool operator>(const ChildNodeEntry<T> &rhs) const {
        return priority > rhs.priority;
    }

    int priority = 0;
    std::shared_ptr<T> child;
};

class Node : public INode {
public:
    Node() = default;

    void draw(const Transform &transform) const final;

    bool handleEvent(const SDL_Event &event) final;

    void update(uint64_t deltaMs) final;

    virtual void onDraw(const glm::mat4 &transform);

    virtual bool onHandleEvent(const SDL_Event &event);

    virtual void onUpdate(uint64_t deltaMs);

    inline void addDrawable(const std::shared_ptr<Drawable> &drawable, int priority = 0) {
        drawables.emplace(drawable, priority);
    }

    inline void addEventConsumer(const std::shared_ptr<EventConsumer> &consumer, int priority = 0) {
        eventConsumers.emplace(consumer, priority);
    }

    inline void scheduleForUpdates(const std::shared_ptr<UpdateDelegate> &delegate, int priority = 0) {
        updateDelegates.emplace(delegate, priority);
    }

private:
    std::multiset<ChildNodeEntry<Drawable>, std::greater<>> drawables;
    std::multiset<ChildNodeEntry<EventConsumer>, std::greater<>> eventConsumers;
    std::multiset<ChildNodeEntry<UpdateDelegate>, std::greater<>> updateDelegates;
};

#endif //SHITCRAFT_NODE_H
