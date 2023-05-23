//
// Created by egorv on 4/22/2023.
//

#ifndef SHITCRAFT_CACHE_H
#define SHITCRAFT_CACHE_H

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

#include <string>
#include <unordered_map>
#include <stdexcept>

namespace asset::detail {

    namespace detail {
        template<typename T>
        struct DummyResolver {
            inline T operator()(const std::string &key) {
                throw std::out_of_range("blockCache doesn't contain key " + key);
            }
        };
    }

    template<typename... Args>
    struct Cache {
    public:
        template<typename T, typename Resolver = detail::DummyResolver<T>>
        const T &get(const std::string &key, Resolver &&resolver = {}) const = delete;

        template <typename T>
        void insert(std::string &&key, T &&value) = delete;
    };

    template<typename T, typename... Args>
    class Cache<T, Args...> : private Cache<Args...> {
    public:
        Cache() = default;

        template<typename V, typename Resolver = detail::DummyResolver<V>>
        inline const V &get(const std::string &key, Resolver &&resolver = {}) const {
            if constexpr (std::is_same_v<T, V>) {
                auto it = data.find(key);

                if (it == data.end())
                    it = data.emplace(key, resolver(key)).first;

                return it->second;
            } else
                return inherited::template get<V>(key, std::forward<Resolver>(resolver));
        }

        template <typename V>
        void insert(std::string &&key, V &&value) {
            if constexpr (std::is_same_v<T, V>)
                data.emplace(std::forward<std::string>(key), std::forward<V>(value));
            else
                inherited::insert(std::forward<std::string>(key), std::forward<V>(value));
        }
    private:
        using inherited = Cache<Args...>;

        mutable std::unordered_map<std::string, T> data;
    };

} // asset::detail

#pragma clang diagnostic pop

#endif //SHITCRAFT_CACHE_H
