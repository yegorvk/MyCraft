//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_VERTEX_H
#define SHITCRAFT_VERTEX_H

#include "glm/glm.hpp"

struct Vertex {
    constexpr Vertex(glm::vec3 position, glm::vec2 texCoords) : position(position), texCoord(texCoords) {}

    glm::vec3 position;
    glm::vec2 texCoord;
};


#endif //SHITCRAFT_VERTEX_H
