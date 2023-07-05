//
// Created by egorv on 4/23/2023.
//

#ifndef USE_CMRC

#include "LocalFileReader.h"
#include "utils/IoUtils.h"

namespace asset {

    LocalFileReader::LocalFileReader(std::string_view directory) : directory(directory) {}

    bool LocalFileReader::exists(std::string_view path) const {
        return std::filesystem::exists(getPath(path));
    }

    std::string LocalFileReader::getText(std::string_view path) {
        return readTextFromFile(getPath(path).generic_string().c_str());
    }

    std::vector<unsigned char> LocalFileReader::getBytes(std::string_view path) {
        return readBytesFromFile(getPath(path).generic_string().c_str());
    }

    std::filesystem::path LocalFileReader::getPath(std::string_view relPath) const {
        auto dirCopy = directory;
        dirCopy.append(relPath);

        return dirCopy;
    }

} // asset

#endif