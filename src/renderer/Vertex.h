//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_VERTEX_H
#define SHITCRAFT_VERTEX_H

#include "glm/glm.hpp"

struct Vertex {
    constexpr Vertex() = default;

    constexpr Vertex(glm::vec3 position, glm::vec3 texCoords, glm::vec3 normal, glm::vec3 color = {1.f, 1.f, 1.f})
            : position(position), texCoords(texCoords), color(color), normal(normal) {}

    glm::vec3 position{};
    glm::vec3 texCoords{};
    glm::vec3 color{};
    glm::vec3 normal{};
};


#endif //SHITCRAFT_VERTEX_H
