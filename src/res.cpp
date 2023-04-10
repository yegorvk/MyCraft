//
// Created by egorv on 4/6/2023.
//

#include "res.h"
#include "config.h"
#include <fstream>
#include <sstream>

#ifndef DEBUG
#include "cmrc/cmrc.hpp"

CMRC_DECLARE(res);

static auto fs = cmrc::res::get_filesystem();
#endif

namespace res {
#ifdef DEBUG
    std::string loadString(const char* path) {
        std::ifstream is(RESOURCES_DIR + std::string(path));
        std::ostringstream oss;
        oss << is.rdbuf();
        return oss.str();
    }
#else
    std::string loadString(const char* path) {
        auto file = fs.open(RESOURCES_PREFIX + std::string(path));
        return {file.begin(), file.end()};
    }
#endif

}