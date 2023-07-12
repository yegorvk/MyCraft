//
// Created by egorv on 7/6/2023.
//

#include "RectMesh.h"

static constexpr SimpleVertex vertices[6] = {
        {glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0, 0)},
        {glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1, 0)},
        {glm::vec3(0.5f, 0.5f, 0.f),   glm::vec2(1, 1)},
        {glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0, 0)},
        {glm::vec3(0.5f, 0.5f, 0.f),   glm::vec2(1, 1)},
        {glm::vec3(-0.5f, 0.5f, 0.f),  glm::vec2(0, 1)},
};

RectMesh::RectMesh() : SimpleMesh(6, vertices) {}