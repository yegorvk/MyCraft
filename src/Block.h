//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_BLOCK_H
#define SHITCRAFT_BLOCK_H

struct Block {
    inline Block(const char *texture, bool translucent) : texture(texture), translucent(translucent) {}

    const char *texture;
    bool translucent;
};

#endif //SHITCRAFT_BLOCK_H
