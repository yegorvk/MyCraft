//
// Created by egorv on 4/23/2023.
//

#include "LocalFileReader.h"
#include "utils.h"

namespace asset {

    LocalFileReader::LocalFileReader(std::string_view directory) : directory(directory) {}

    std::string LocalFileReader::getText(std::string_view path) {
        return readTextFromFile(getPath(path).generic_string().c_str());
    }

    std::vector<unsigned char> LocalFileReader::getBytes(std::string_view path) {
        return readBytesFromFile(getPath(path).generic_string().c_str());
    }

    std::filesystem::path LocalFileReader::getPath(std::string_view relPath) {
        auto dirCopy = directory;
        dirCopy.append(relPath);

        return std::move(dirCopy);
    }

} // asset