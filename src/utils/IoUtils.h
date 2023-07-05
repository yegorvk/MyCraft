//
// Created by egorv on 4/12/2023.
//

#ifndef SHITCRAFT_IOUTILS_H
#define SHITCRAFT_IOUTILS_H

#include <string>
#include <vector>

#include "glm/glm.hpp"

std::vector<uint8_t> readBytesFromFile(const char* path);

std::string readTextFromFile(const char* path);

#endif //SHITCRAFT_IOUTILS_H
