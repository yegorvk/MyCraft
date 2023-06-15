//
// Created by egorv on 4/5/2023.
//

#ifndef SHITCRAFT_GAME_H
#define SHITCRAFT_GAME_H

#include "SDL.h"
#include "glad/glad.h"
#include "core/Node.h"
#include "arch.h"

class Game {
public:
    Game(const char *winTitle, int winWidth, int winHeight, bool fullscreen);

    ~Game();

    void run();

private:
    // Initialization functions

    static void initSDL();

    static void setGlContextSettings();

    void createWindow(const char *winTitle, int winWidth, int winHeight, bool fullscreen);

    void createGlContext();

    void onWindowGlContextReady();

    // Callbacks

#ifdef DEBUG
    static void STDCALL processGlDebugMessage(
            GLuint source,
            GLuint type,
            GLuint id,
            GLuint severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam);
#endif

    // Helpers

    [[noreturn]]
    static void die(const char *msg);

    // Data

    std::unique_ptr<Node> rootNode;

    SDL_Window *window = nullptr;
    SDL_GLContext glContext = nullptr;
};

#endif //SHITCRAFT_GAME_H
