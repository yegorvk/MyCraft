//
// Created by egorv on 4/6/2023.
//

#ifndef SHITCRAFT_SHADER_H
#define SHITCRAFT_SHADER_H

#include "types.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad/gl.h"

class Shader {
public:
    static Shader compile(const char* vertShaderSrc, const char* fragShaderSrc);

    inline explicit Shader(uint handle): handle(handle) {};

    ~Shader();

    inline void bind() const {
        glUseProgram(handle);
    }

    static inline void unbind() {
        glUseProgram(0);
    }

    inline void setFloat(const char* name, float value) const {
        glUniform1f(getLoc(name), value);
    }

    inline void setVec2(const char* name, const glm::vec2& value) const {
        glUniform2fv(getLoc(name), 1, glm::value_ptr(value));
    }

    inline void setVec3(const char* name, glm::vec3 value) const {
        glUniform3fv(getLoc(name), 1, glm::value_ptr(value));
    }

    inline void setVec4(const char* name, glm::vec4 value) const {
        glUniform4fv(getLoc(name), 1, glm::value_ptr(value));
    }

    inline void setMat2(const char* name, glm::mat4 value) const {
        glUniformMatrix2fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    inline void setMat3(const char* name, glm::mat4 value) const {
        glUniformMatrix3fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    inline void setMat4(const char* name, glm::mat4 value) const {
        glUniformMatrix4fv(getLoc(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    [[nodiscard]]
    explicit inline operator bool() const {
        return handle != 0;
    }
private:
    inline int getLoc(const char* name) const {
        return glGetUniformLocation(handle, name);
    }

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
