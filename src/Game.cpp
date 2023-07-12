//
// Created by egorv on 4/5/2023.
//

#include "Game.h"
#include "spdlog/spdlog.h"
#include "config.h"
#include "SDL.h"
#include "GameContext.h"
#include "arch.h"
#include "scene/WorldScene.h"

#define glGetStr(prop) reinterpret_cast<const char*>(glGetString(prop))

Game::Game(const char *winTitle, int winWidth, int winHeight, bool fullscreen) {
    initSDL();
    setGlContextSettings();
    createWindow(winTitle, winWidth, winHeight, fullscreen);
    createGlContext();
    onWindowGlContextReady();
}

Game::~Game() {
    rootNode.reset();

    GameContext::destroyGlobal();

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
    glClearColor(0.f, 0.f, 0.f, 0.f);

    bool quit = false;
    SDL_Event event;

    int winWidth, winHeight;
    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);

    SDL_Event resizeEvent;

    resizeEvent.type = SDL_WINDOWEVENT;
    resizeEvent.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
    resizeEvent.window.data1 = winWidth;
    resizeEvent.window.data2 = winHeight;

    rootNode->handleEvent(resizeEvent);

    auto lastFrameTimestamp = SDL_GetTicks64();

    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!quit) {
        auto curFrameTime = SDL_GetTicks64();
        auto delta = curFrameTime - lastFrameTimestamp;
        lastFrameTimestamp = curFrameTime;

//        float fps = 1000.f / static_cast<float>(delta);
//        spdlog::debug("Fps: {}", fps);

        SDL_GL_SwapWindow(window);
        SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);

            if (rootNode)
                rootNode->handleEvent(event);

            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
                quit = true;
        }

        rootNode->update(delta);

        glViewport(0, 0, winWidth, winHeight);

        if (rootNode)
            rootNode->draw();
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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

#ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
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

    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress));

    spdlog::info("OpenGL loaded correctly.");

    spdlog::info("Vendor: {}", glGetStr(GL_VENDOR));
    spdlog::info("Renderer: {}", glGetStr(GL_RENDERER));
    spdlog::info("Version: {}", glGetStr(GL_VERSION));

    SDL_GL_SetSwapInterval(1);
}

void Game::onWindowGlContextReady() {
    GameContext::setGlobal(new GameContext());

    int winWidth, winHeight;
    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);
    glViewport(0, 0, winWidth, winHeight);

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW_ARB, 0, nullptr, GL_TRUE);
    glDebugMessageCallbackARB(Game::processGlDebugMessage, nullptr);
#endif

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    rootNode = std::make_unique<WorldScene>();
}

[[noreturn]]
void Game::die(const char *msg) {
    spdlog::error("{}. SDL error: {}", msg, SDL_GetError());
    exit(EXIT_FAILURE);
}

#ifdef DEBUG

void STDCALL Game::processGlDebugMessage(GLuint source, GLuint type, GLuint id, GLuint severity, GLsizei,
                                         const GLchar *message, const void *) {
    spdlog::level::level_enum logLevel;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH_ARB:
            logLevel = spdlog::level::err;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM_ARB:
            logLevel = spdlog::level::warn;
            break;
        case GL_DEBUG_SEVERITY_LOW_ARB:
            logLevel = spdlog::level::info;
            break;
        default:
            logLevel = spdlog::level::debug;
    }

    if (logLevel <= spdlog::level::debug)
        return;

    const char *sourceStr;

    switch (source) {
        case GL_DEBUG_SOURCE_API_ARB:
            sourceStr = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
            sourceStr = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
            sourceStr = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
            sourceStr = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION_ARB:
            sourceStr = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER_ARB:
            sourceStr = "OTHER";
            break;
        default:
            sourceStr = "UNKNOWN";
    }

    const char *typeStr;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB:
            typeStr = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
            typeStr = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
            typeStr = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY_ARB:
            typeStr = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE_ARB:
            typeStr = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER_ARB:
            typeStr = "OTHER";
            break;
        default:
            typeStr = "UNKNOWN";
    }

    spdlog::log(logLevel, "OpenGL debug message (id: {}, source {}, type: {}): {}", id, sourceStr, typeStr, message);
}

#endif DEBUG