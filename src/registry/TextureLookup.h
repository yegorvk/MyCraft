//
// Created by egorv on 6/3/2023.
//

#ifndef SHITCRAFT_TEXTURELOOKUP_H
#define SHITCRAFT_TEXTURELOOKUP_H

#include <map>
#include <string>

using TextureId = uint16_t;

class TextureLookup {
public:
    TextureLookup() = default;

    inline void registerTexture(const std::string &key, TextureId id) {
        lookup.emplace(key, id);
    }

    [[nodiscard]] inline TextureId getId(const std::string &key) const {
        return lookup.at(key);
    }

protected:
    [[nodiscard]] inline bool contains(const std::string &key) const {
        return lookup.count(key);
    }
private:
    std::map<std::string, TextureId> lookup;
};

#endif //SHITCRAFT_TEXTURELOOKUP_H
