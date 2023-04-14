//
// Created by egorv on 4/11/2023.
//

#ifndef SHITCRAFT_ASSETMANAGER_H
#define SHITCRAFT_ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include "Texture.h"
#include "config.h"

class AssetProvider {
public:
    virtual const std::string &getStr(const char *path) = 0;

    virtual const Texture2D &getTex2D(const char *path, TexSamplerOptions options) = 0;

    inline const Texture2D &getTex2D(const char *path) {
        return getTex2D(path, TexSamplerOptions());
    }
};

class AssetLoader {
public:
    virtual ~AssetLoader() = default;

    virtual std::string loadStr(const char *path) = 0;

    virtual Texture2D loadTex2D(const char *path, TexSamplerOptions options) = 0;
};

namespace assetManagerImpl {
    template<typename T>
    class AssetCache {
    public:
        AssetCache() = default;

        template<typename LoadFunc>
        const T &get(const char *path, LoadFunc load) {
            auto it = entries.find(path);

            if (it != entries.end())
                return it->second;
            else
                return entries.emplace(path, load(path)).first->second;
        }
    private:
        std::unordered_map<std::string, T> entries;
    };
}

class LocalFsAssetLoader : public AssetLoader {
public:
    inline explicit LocalFsAssetLoader(std::string &&rootDir) : rootDir(rootDir) {}

#ifdef DEBUG
    inline LocalFsAssetLoader() : rootDir(RESOURCES_DIR) {}
#endif

    std::string loadStr(const char *path) final;

    Texture2D loadTex2D(const char *path, TexSamplerOptions options) final;
private:
    std::string rootDir;
};

#ifdef DEBUG
using DefaultAssetLoader = LocalFsAssetLoader;
#endif

class AssetManager : public AssetProvider {
public:
    inline explicit AssetManager(AssetLoader &loader) : loader(loader) {}

    inline const std::string &getStr(const char *path) final {
        return strings.get(path, [this] (const char* path) {
            return loader.loadStr(path);
        });
    }

    inline const Texture2D &getTex2D(const char *path, TexSamplerOptions options) final {
        return tex2Ds.get(path, [this, options] (const char* path) {
            return std::move(loader.loadTex2D(path, options));
        });
    }
private:
    AssetLoader &loader;

    assetManagerImpl::AssetCache<std::string> strings;
    assetManagerImpl::AssetCache<Texture2D> tex2Ds;
};

#endif //SHITCRAFT_ASSETMANAGER_H
