//
// Created by egorv on 4/6/2023.
//

#ifndef SHITCRAFT_SHADER_H
#define SHITCRAFT_SHADER_H

#include "types.h"

class Shader {
public:
    static Shader compile(const char* vertShaderSrc, const char* fragShaderSrc);

    inline explicit Shader(uint handle): handle(handle) {};

    void bind();

    static void unbind();

    ~Shader();

    [[nodiscard]]
    explicit inline operator bool() const {
        return handle != 0;
    }
private:
    uint handle = 0;
};

enum class ShaderType {
    VERTEX = 0,
    FRAGMENT = 1
};

constexpr int SHADER_STAGE_COUNT = 2;

class ShaderBuilder {
public:
    ShaderBuilder() = default;

    ~ShaderBuilder();

    bool setStage(ShaderType type, const char* src);

    Shader link();
private:
    uint stages[SHADER_STAGE_COUNT]{};
};

#endif //SHITCRAFT_SHADER_H
