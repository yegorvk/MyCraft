//
// Created by egorv on 4/23/2023.
//

#ifndef SHITCRAFT_LOCALFILEREADER_H
#define SHITCRAFT_LOCALFILEREADER_H

#include <filesystem>

#include "config.h"
#include "FileReader.h"

namespace asset {

    class LocalFileReader : public FileReader {
    public:
        explicit LocalFileReader(std::string_view directory = ASSETS_DIR);

        std::string getText(std::string_view path) final;

        std::vector<unsigned char> getBytes(std::string_view path) final;

    private:
        std::filesystem::path getPath(std::string_view relPath);

        std::filesystem::path directory;
    };

} // asset



#endif //SHITCRAFT_LOCALFILEREADER_H
