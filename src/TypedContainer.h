//
// Created by egorv on 5/7/2023.
//

#ifndef SHITCRAFT_TYPEDcontainer_type_H
#define SHITCRAFT_TYPEDcontainer_type_H

#include <type_traits>

template<typename ContainerTypeProvider, typename ...Types>
class TypedContainer {
public:
    template<typename T>
    using container_type = typename ContainerTypeProvider::template type<T>;

    template<typename T>
    container_type<T> &get() = delete;

    template<typename T>
    [[nodiscard]] const container_type<T> &get() const = delete;
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

template<typename ContainerTypeProvider, typename Type, typename ...Types>
class TypedContainer<ContainerTypeProvider, Type, Types...> : private TypedContainer<ContainerTypeProvider, Types...> {
public:
    template<typename T>
    using container_type = typename ContainerTypeProvider::template type<T>;

    template<typename T>
    container_type<T> &get() {
        if constexpr (std::is_same_v<Type, T>)
            return container;
        else
            return TypedContainer<ContainerTypeProvider, Types...>::template get<T>();
    }

    template<typename T>
    const container_type<T> &get() const {
        if constexpr (std::is_same_v<Type, T>)
            return container;
        else
            return TypedContainer<ContainerTypeProvider, Types...>::template get<T>();
    }

private:
    container_type<Type> container;
};

#pragma clang diagnostic pop

#endif //SHITCRAFT_TYPEDcontainer_type_H
