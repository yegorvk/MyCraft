//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_PROVIDER_H
#define SHITCRAFT_PROVIDER_H

#include <string>
#include "renderer/Texture.h"
#include "renderer/Shader.h"

namespace asset {

    class Provider {
    public:
        virtual ~Provider() = default;

        [[nodiscard]] virtual const std::string &getText(const std::string &key) const = 0;

        [[nodiscard]] virtual const Texture &getTexture(const std::string &key) const = 0;

        [[nodiscard]] virtual const Shader &getShader(const std::string &key) const = 0;
    };
}


#endif //SHITCRAFT_PROVIDER_H
