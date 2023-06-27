//
// Created by egorv on 6/26/2023.
//

#ifndef SHITCRAFT_EXECUTOR_H
#define SHITCRAFT_EXECUTOR_H

#include "BS_thread_pool.hpp"

class Executor {
public:
    Executor();

    [[nodiscard]] static Executor &global();

    template<typename Task, typename ...Args, typename Result = void>
    inline std::future<Result> submit(Task &&task, Args &&...args) {
        return pool.submit(std::forward<Task>(task), std::forward<Args>(args)...);
    }

    inline void waitForTasks() {
        pool.wait_for_tasks();
    }

private:
    BS::thread_pool pool;
};


#endif //SHITCRAFT_EXECUTOR_H
