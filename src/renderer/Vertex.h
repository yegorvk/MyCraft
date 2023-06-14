//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_VERTEX_H
#define SHITCRAFT_VERTEX_H

#include "glm/glm.hpp"

struct Vertex {
    constexpr Vertex() = default;

    constexpr Vertex(glm::vec3 position, glm::vec2 texCoords, glm::vec3 color, int textureId)
            : position(position), texCoords(texCoords), color(color), textureId(textureId) {}

    glm::vec3 position{};
    glm::vec2 texCoords{};
    glm::vec3 color{};

    int textureId{};
};

#endif //SHITCRAFT_VERTEX_H
