//
// Created by egorv on 4/6/2023.
//

#include "Shader.h"
#include "glad/gl.h"
#include "spdlog/spdlog.h"

Shader Shader::compile(const char *vertShaderSrc, const char *fragShaderSrc) {
    ShaderBuilder builder;

    if (!builder.setStage(ShaderType::VERTEX, vertShaderSrc))
        return Shader(0);

    if (!builder.setStage(ShaderType::FRAGMENT, fragShaderSrc))
        return Shader(0);

    return builder.link();
}

void Shader::bind() {
    glUseProgram(handle);
}

void Shader::unbind() {
    glUseProgram(0);
}

Shader::~Shader() {
    if (handle != 0)
        glDeleteProgram(handle);
}

static const char *shaderTypeStr[SHADER_STAGE_COUNT] = {"VERTEX", "FRAGMENT"};
constexpr int glShaderType[SHADER_STAGE_COUNT] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};

ShaderBuilder::~ShaderBuilder() {
    for (uint stage : stages) {
        if (stage != 0)
            glDeleteShader(stage);
    }
}

bool ShaderBuilder::setStage(ShaderType type, const char *src) {
    int stage = static_cast<int>(type);

    if (stages[stage] != 0) {
        glDeleteShader(stages[stage]);
        stages[stage] = 0;
    }

    uint shader = glCreateShader(glShaderType[stage]);

    if (shader == 0)
        return false;

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int compileStatus = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (!compileStatus) {
        int infoLogLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        char *infoLog = new char[infoLogLen];
        glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLog);

        spdlog::error("{} shader compilation failed.\nCompiler output:\n{}", shaderTypeStr[stage],
                      const_cast<const char *>(infoLog));

        delete[] infoLog;

        return false;
    } else
        stages[stage] = shader;

    return true;
}

Shader ShaderBuilder::link() {
    uint program = glCreateProgram();

    if (program == 0)
        return Shader(0);

    for (uint stage : stages) {
        if (stage != 0)
            glAttachShader(program, stage);
    }

    glLinkProgram(program);

    int linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (!linkStatus) {
        int infoLogLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);

        char *infoLog = new char[infoLogLen];
        glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog);

        spdlog::error("Program linking failed. Linker output: \n{}", const_cast<const char *>(infoLog));

        delete[] infoLog;

        return Shader(0);
    }

    return Shader(program);
}
