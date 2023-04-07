//
// Created by egorv on 4/6/2023.
//

#include "res.h"
#include "cmrc/cmrc.hpp"

CMRC_DECLARE(res);

static auto fs = cmrc::res::get_filesystem();

namespace res {
    std::string getString(const char* path) {
        auto file = fs.open(path);
        return {file.begin(), file.end()};
    }
}