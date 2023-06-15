//
// Created by egorv on 6/14/2023.
//

#if !defined(SHITCRAFT_CMRCFILEREADER_H) && defined(USE_CMRC)
#define SHITCRAFT_CMRCFILEREADER_H

#include "cmrc/cmrc.hpp"

#include "config.h"
#include "FileReader.h"

namespace asset {
    class CmrcFileReader : public FileReader {
    public:
        CmrcFileReader();

        [[nodiscard]] bool exists(std::string_view path) const final;

        std::string getText(std::string_view path) final;

        std::vector<unsigned char> getBytes(std::string_view path) final;

    private:
        static std::string getPath(std::string_view relPath);

        cmrc::embedded_filesystem fs;
    };
}

#endif //SHITCRAFT_CMRCFILEREADER_H
