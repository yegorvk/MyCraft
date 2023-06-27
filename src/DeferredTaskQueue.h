//
// Created by egorv on 6/27/2023.
//

#ifndef SHITCRAFT_DEFERREDTASKQUEUE_H
#define SHITCRAFT_DEFERREDTASKQUEUE_H

#include <queue>
#include <stack>
#include <optional>

template<typename T>
class DeferredTaskQueue {
public:
    // Opaque task group handle
    using task_group_id_type = int;

    DeferredTaskQueue() = default;

    // Pushes element into the queue and assigns it to the given group.
    // Returns new element's group handle that is guaranteed to be the same as the one provided
    // as long as it remains valid.
    inline task_group_id_type enqueue(T elem, task_group_id_type groupId) {
        groups[groupId].tasks.push(std::move(elem));
        return groupId;
    }

    // Pushes element into the queue and returns its group handle
    inline task_group_id_type enqueue(T elem) {
        return enqueue(std::move(elem), findEmptyGroup());
    }

    // Returns the front element of the queue that belongs to the given group if it was previously enabled.
    // Different groups are processed independently, so they might appear in any order.
    std::optional<T> dequeue(task_group_id_type groupId) {
        if (!groups[groupId].active || groups[groupId].groups.empty())
            return std::nullopt;
        else {
            T task = std::move(groups[groupId].groups.front());
            groups[groupId].groups.pop();

            if (groups[groupId].groups.empty())
                recycleGroup(groupId);

            return task;
        }
    }

    // Marks group as enabled and invalidates its group handle for enqueue-ing
    // After calling this function group handle must not be used for enqueue operations
    inline void enableGroup(task_group_id_type groupId) {
        groups[groupId].active = true;
    }

private:
    struct TaskGroup {
        std::queue<T> tasks;
        bool active = false;
    };

    std::vector<TaskGroup> groups;
    std::stack<task_group_id_type> emptyGroups;

    task_group_id_type findEmptyGroup() {
        if (emptyGroups.empty()) {
            groups.emplace_back();
            return static_cast<task_group_id_type>(groups.size() - 1);
        } else {
            auto group = emptyGroups.top();
            emptyGroups.pop();
            return group;
        }
    }

    // Assumes no pending groups
    inline void recycleGroup(task_group_id_type groupId) {
        groups[groupId].active = false;
        emptyGroups.push(groups[groupId]);
    }
};

#endif //SHITCRAFT_DEFERREDTASKQUEUE_H
