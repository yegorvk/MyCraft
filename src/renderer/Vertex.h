//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_VERTEX_H
#define SHITCRAFT_VERTEX_H

#include "glm/glm.hpp"

struct Vertex {
    constexpr Vertex() = default;
    constexpr Vertex(glm::vec3 position, glm::vec3 texCoords) : position(position), texCoords(texCoords) {}

    glm::vec3 position{};
    glm::vec3 texCoords{};
};


#endif //SHITCRAFT_VERTEX_H
