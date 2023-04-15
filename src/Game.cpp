//
// Created by egorv on 4/5/2023.
//

#define GLM_FORCE_RADIANCE

#include "Game.h"
#include "spdlog/spdlog.h"
#include "renderer/Shader.h"
#include "config.h"
#include "SDL.h"
#include "camera/CameraControls.h"
#include "Context.h"
#include "config.h"

#include "SimpleCubeScene.h"
#include "arch.h"

#define glGetStr(prop) reinterpret_cast<const char*>(glGetString(prop))

constexpr float P_FOV_RAD = glm::pi<float>() / 3.f;
constexpr float P_NEAR = 0.001f;
constexpr float P_FAR = 1000.f;

Game::Game(const char *winTitle, int winWidth, int winHeight, bool fullscreen) {
    initSDL();
    setGlContextSettings();
    createWindow(winTitle, winWidth, winHeight, fullscreen);
    createGlContext();
    onWindowGlContextReady();
}

Game::~Game() {
    rootNode.reset();

    Context::destroyGlobal();

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
    glClearColor(1.f, 1.f, 1.f, 1.f);

    bool quit = false;
    SDL_Event event;

    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);

    float aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
    glm::mat4 projMat = glm::perspective(P_FOV_RAD, aspect, P_NEAR, P_FAR);

    auto lastFrameTimestamp = SDL_GetTicks64();

    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!quit) {
        auto curFrameTime = SDL_GetTicks();
        auto delta = curFrameTime - lastFrameTimestamp;
        lastFrameTimestamp = curFrameTime;

        SDL_GL_SwapWindow(window);
        SDL_GetWindowSize(window, &winWidth, &winHeight);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                quit = true;

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                winWidth = event.window.data1, winHeight = event.window.data2;

                aspect = static_cast<float>(winWidth) / static_cast<float>(winHeight);
                projMat = glm::perspective(P_FOV_RAD, aspect, P_NEAR, P_FAR);

                glViewport(0, 0, event.window.data1, event.window.data2);
            }

            if (rootNode)
                rootNode->handleEvent(event);
        }

        rootNode->update(delta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Transform transform(projMat * camera.getViewMatrix());

        if (rootNode)
            rootNode->draw(transform);
    }

    SDL_SetRelativeMouseMode(SDL_FALSE);
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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

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

void Game::onWindowGlContextReady() {
    Context::setGlobal(new Context());

    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);
    glViewport(0, 0, winWidth, winHeight);

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(Game::processGlDebugMessage, nullptr);
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    rootNode = std::make_unique<SimpleCubeScene>();

    camera.moveAbsolute(glm::vec3(0.f, 0.f, 3.f));

    auto camControls = std::make_shared<CameraControls>(camera);

    rootNode->scheduleForUpdates(camControls);
    rootNode->addEventConsumer(camControls);
}

[[noreturn]]
void Game::die(const char *msg) {
    spdlog::error("{}. SDL error: {}", msg, SDL_GetError());
    exit(EXIT_FAILURE);
}

void STDCALL Game::processGlDebugMessage(GLuint source, GLuint type, GLuint id, GLuint severity, GLsizei length,
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

    const char *sourceStr;

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

    const char *typeStr;

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
