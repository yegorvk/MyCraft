//
// Created by egorv on 4/5/2023.
//

#include "Game.h"
#include "spdlog/spdlog.h"
#include "Shader/Shader.h"

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

        // TODO drawing logic
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

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
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
}

void Game::handleEvent(const SDL_Event &event) {

}

[[noreturn]]
void Game::die(const char *msg) {
    spdlog::error("{}. SDL error: {}", msg, SDL_GetError());
    exit(EXIT_FAILURE);
}
