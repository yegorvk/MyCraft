//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_NODE_H
#define SHITCRAFT_NODE_H

#include "INode.h"
#include <set>
#include <memory>
#include <optional>

template<typename T>
struct ChildNodeEntry {
    inline explicit ChildNodeEntry(std::shared_ptr<T> child, int priority = 0) : priority(priority),
                                                                                 child(std::move(child)) {}

    inline bool operator>(const ChildNodeEntry<T> &rhs) const {
        return priority > rhs.priority;
    }

    int priority = 0;
    std::shared_ptr<T> child;
};

class Node : public INode {
public:
    Node() = default;

    ~Node() override = default;

    void draw() const final;

    bool handleEvent(const SDL_Event &event) final;

    void update(uint64_t deltaMs) final;

    virtual void onPreDraw() const;

    virtual void onPostDraw() const;

    virtual bool onHandleEvent(const SDL_Event &event);

    virtual void onUpdate(uint64_t deltaMs);

    inline void addDrawable(std::shared_ptr<Drawable> drawable, int priority = 0) {
        drawables.emplace(std::move(drawable), priority);
    }

    inline void addEventConsumer(std::shared_ptr<EventConsumer> consumer, int priority = 0) {
        eventConsumers.emplace(std::move(consumer), priority);
    }

    inline void scheduleForUpdates(std::shared_ptr<UpdateDelegate> delegate, int priority = 0) {
        updateDelegates.emplace(std::move(delegate), priority);
    }

private:
    std::multiset<ChildNodeEntry<Drawable>, std::greater<>> drawables;
    std::multiset<ChildNodeEntry<EventConsumer>, std::greater<>> eventConsumers;
    std::multiset<ChildNodeEntry<UpdateDelegate>, std::greater<>> updateDelegates;
};

#endif //SHITCRAFT_NODE_H
