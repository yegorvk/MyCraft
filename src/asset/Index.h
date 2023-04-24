//
// Created by egorv on 4/23/2023.
//

#ifndef SHITCRAFT_INDEX_H
#define SHITCRAFT_INDEX_H

#include "Cache.h"
#include "Asset.h"

namespace asset {

    class Index {
    public:
        template<typename T>
        [[nodiscard]] inline const T &get(const std::string &key) const {
            return assets.get<T>(key);
        }

        template<typename T>
        inline void insert(std::string &&key, T &&value) {
            assets.insert(std::forward<std::string>(key), std::forward<T>(value));
        }

    private:
        detail::Cache<TextAsset, ImageAsset, ShaderAsset> assets;
    };

}


#endif //SHITCRAFT_INDEX_H
