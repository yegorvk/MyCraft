//
// Created by egorv on 6/26/2023.
//

#include "Executor.h"

static Executor GLOBAL;

Executor::Executor() : pool(std::thread::hardware_concurrency()) {}

Executor &Executor::global() {
    return GLOBAL;
}