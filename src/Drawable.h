//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_DRAWABLE_H
#define SHITCRAFT_DRAWABLE_H

#include "Transform.h"

struct Drawable {
    virtual void draw(const Transform &transform) const = 0;
};

#endif //SHITCRAFT_DRAWABLE_H
