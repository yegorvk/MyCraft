//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_EVENTCONSUMER_H
#define SHITCRAFT_EVENTCONSUMER_H

#include "SDL.h"

struct EventConsumer {
    virtual bool handleEvent(const SDL_Event& event) = 0;
};

#endif //SHITCRAFT_EVENTCONSUMER_H
