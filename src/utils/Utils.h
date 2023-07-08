//
// Created by egorv on 7/6/2023.
//

#ifndef SHITCRAFT_UTILS_H
#define SHITCRAFT_UTILS_H

#define BUF_PTR(STRUCT, MEMBER) reinterpret_cast<void *>(offsetof(STRUCT, MEMBER))

#endif //SHITCRAFT_UTILS_H
