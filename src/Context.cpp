//
// Created by egorv on 4/14/2023.
//

#include "Context.h"

Context *GLOBAL_CONTEXT = nullptr;

Context &Context::global() {
    return *GLOBAL_CONTEXT;
}

void Context::setGlobal(Context *context) {
    GLOBAL_CONTEXT = context;
}

void Context::destroyGlobal() {
    delete GLOBAL_CONTEXT;
    GLOBAL_CONTEXT = nullptr;
}


