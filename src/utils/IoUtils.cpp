//
// Created by egorv on 4/12/2023.
//

#include "IoUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<uint8_t> readBytesFromFile(const char *path) {
    std::ifstream is;
    is.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);

    is.open(path, std::ios_base::binary);

    is.seekg(0, std::ios_base::end);
    auto size = is.tellg();
    is.seekg(0, std::ios_base::beg);

    std::vector<uint8_t> buf(size);
    is.read(reinterpret_cast<char *>(buf.data()), size);

    return buf;
}

std::string readTextFromFile(const char *path) {
    std::ifstream is;
    is.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);

    is.open(path);

    std::ostringstream ss;
    ss << is.rdbuf();

    return ss.str();
}