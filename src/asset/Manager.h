//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_MANAGER_H
#define SHITCRAFT_MANAGER_H

#include <unordered_map>
#include <memory>

#include "Provider.h"
#include "Cache.h"
#include "Asset.h"
#include "Index.h"
#include "FileReader.h"

namespace asset {

    class Manager : public Provider {
    public:
        static Manager createDefault(std::unique_ptr<FileReader> reader);

        Manager(Index &&index, std::unique_ptr<FileReader> reader);

        const std::string &getText(const std::string &key) const final;

        const Image &getImage(const std::string &key) const final;

        const Shader &getShader(const std::string &key) const final;

    private:
        template<typename T>
        struct Resolver {
            inline explicit Resolver(const Manager &enclosing) : enclosing(enclosing) {};

            inline T operator()(const std::string &key) {
                return enclosing.resolve<T>(key);
            }

        private:
            const Manager &enclosing;
        };

        template<typename T>
        T resolve(const std::string &ref) const;

        Index index;
        std::unique_ptr<FileReader> reader;
        mutable detail::Cache<std::string, Image, Shader> cache;
    };

}

#endif //SHITCRAFT_MANAGER_H
