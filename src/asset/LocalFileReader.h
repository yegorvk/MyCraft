//
// Created by egorv on 4/23/2023.
//

#if !defined(SHITCRAFT_LOCALFILEREADER_H) && !defined(USE_CMRC)
#define SHITCRAFT_LOCALFILEREADER_H

#include <filesystem>

#include "config.h"
#include "FileReader.h"

namespace asset {

    class LocalFileReader : public FileReader {
    public:
        explicit LocalFileReader(std::string_view directory = ASSETS_DIR);

        [[nodiscard]] bool exists(std::string_view path) const final;

        std::string getText(std::string_view path) final;

        std::vector<unsigned char> getBytes(std::string_view path) final;

    private:
        [[nodiscard]] std::filesystem::path getPath(std::string_view relPath) const;

        std::filesystem::path directory;
    };

} // asset



#endif //SHITCRAFT_LOCALFILEREADER_H
