//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_FILEREADER_H
#define SHITCRAFT_FILEREADER_H

#include <string>
#include <vector>
#include <filesystem>

namespace asset {

    class FileReader {
    public:
        virtual ~FileReader() = default;

        virtual std::string getText(std::string_view path) = 0;

        virtual std::vector<unsigned char> getBytes(std::string_view path) = 0;
    };

}

#endif //SHITCRAFT_FILEREADER_H
