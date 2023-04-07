//
// Created by egorv on 4/5/2023.
//

#include "Game.h"
#include "spdlog/spdlog.h"
#include "Shader/Shader.h"
#include "res.h"
#include "config.h"

#define glGetStr(prop) reinterpret_cast<const char*>(glGetString(prop))

Game::Game(const char *winTitle, int winWidth, int winHeight, bool fullscreen) {
    initSDL();
    setGlContextSettings();
    createWindow(winTitle, winWidth, winHeight, fullscreen);
    createGlContext();
}

Game::~Game() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void Game::run() {
    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);
    glViewport(0, 0, winWidth, winHeight);

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(Game::processGlDebugMessage, nullptr);
#endif

    auto vertShaderSrc = res::getString("res/glsl/basic.vert");
    auto fragShaderSrc = res::getString("res/glsl/basic.frag");

    auto shader = Shader::compile(vertShaderSrc.c_str(), fragShaderSrc.c_str());

    float vertices[] = {
            -0.5f, -0.5f,
            0.f, 0.5f,
            0.5f, -0.5f
    };

    uint vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint vao = 0;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(1.f, 1.f, 1.f, 1.f);

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        SDL_GL_SwapWindow(window);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;
            else
                handleEvent(event);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        Shader::unbind();
    }
}

void Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        die("Couldn't initialize SDL");
}

void Game::setGlContextSettings() {
    SDL_GL_LoadLibrary(nullptr);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void Game::createWindow(const char *winTitle, int winWidth, int winHeight, bool fullscreen) {
    if (fullscreen)
        window = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    else
        window = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, winWidth,
                                  winHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == nullptr)
        die("Cannot create window");
}

void Game::createGlContext() {
    glContext = SDL_GL_CreateContext(window);

    if (glContext == nullptr)
        die("Failed to create OpenGL context");

    gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));

    spdlog::info("OpenGL loaded correctly.");

    spdlog::info("Vendor: {}", glGetStr(GL_VENDOR));
    spdlog::info("Renderer: {}", glGetStr(GL_RENDERER));
    spdlog::info("Version: {}", glGetStr(GL_VERSION));

    SDL_GL_SetSwapInterval(1);
}

void Game::handleEvent(const SDL_Event &event) {
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        glViewport(0, 0, event.window.data1, event.window.data2);
}

[[noreturn]]
void Game::die(const char *msg) {
    spdlog::error("{}. SDL error: {}", msg, SDL_GetError());
    exit(EXIT_FAILURE);
}

void Game::processGlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                 const GLchar *message, const void *userParam) {
    spdlog::level::level_enum logLevel;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            logLevel = spdlog::level::critical;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            logLevel = spdlog::level::err;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            logLevel = spdlog::level::warn;
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            logLevel = spdlog::level::info;
            break;
        default:
            logLevel = spdlog::level::debug;
    }

    const char* sourceStr;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            sourceStr = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceStr = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceStr = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceStr = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceStr = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceStr = "OTHER";
            break;
        default:
            sourceStr = "UNKNOWN";
    }

    const char* typeStr;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeStr = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeStr = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeStr = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeStr = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeStr = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            typeStr = "MARKER";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeStr = "PUSH_GROUP";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeStr = "POP_GROUP";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeStr = "OTHER";
            break;
        default:
            typeStr = "UNKNOWN";
    }

    spdlog::log(logLevel, "OpenGL debug message (id: {}, source {}, type: {}): {}", id, sourceStr, typeStr, message);
}
