//
// Created by egorv on 4/9/2023.
//

#ifndef SHITCRAFT_UPDATEDELEGATE_H
#define SHITCRAFT_UPDATEDELEGATE_H

#include <cstdint>

class UpdateDelegate {
public:
    virtual void update(uint64_t deltaMs) = 0;
};

#endif //SHITCRAFT_UPDATEDELEGATE_H
