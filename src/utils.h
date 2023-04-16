//
// Created by egorv on 4/12/2023.
//

#ifndef SHITCRAFT_UTILS_H
#define SHITCRAFT_UTILS_H

#include <string>
#include <vector>

std::vector<uint8_t> readBytesFromFile(const char* path);

std::string readTextFromFile(const char* path);

#endif //SHITCRAFT_UTILS_H
