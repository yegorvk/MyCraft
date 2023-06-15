//
// Created by egorv on 6/14/2023.
//

#ifdef USE_CMRC

#include <regex>

#include "CmrcFileReader.h"

CMRC_DECLARE(assets);

namespace asset {
    CmrcFileReader::CmrcFileReader() : fs(cmrc::assets::get_filesystem()) {}

    bool CmrcFileReader::exists(std::string_view path) const {
        return fs.exists(getPath(path));
    }

    std::string CmrcFileReader::getText(std::string_view path) {
        auto file = fs.open(getPath(path));

        std::string data(file.size(), '@');
        std::copy(file.begin(), file.end(), data.begin());

        return data;
    }

    std::vector<unsigned char> CmrcFileReader::getBytes(std::string_view path) {
        auto file = fs.open(getPath(path));

        std::vector<unsigned char> data(file.size());
        std::copy(file.begin(), file.end(), data.begin());

        return data;
    }

    std::string CmrcFileReader::getPath(std::string_view relPath) {
        static std::regex regex(R"([\/]+[\.\/]*[\/]+)");

        std::string tmp = ASSETS_PREFIX + std::string(relPath);
        std::reverse(tmp.begin(), tmp.end());

        while (tmp.back() == '.')
            tmp.pop_back();

        std::reverse(tmp.begin(), tmp.end());

        auto t = std::regex_replace(tmp, regex, "/");
        return t;
    }
}

#endif