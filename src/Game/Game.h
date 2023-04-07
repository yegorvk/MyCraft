//
// Created by egorv on 4/5/2023.
//

#ifndef SHITCRAFT_GAME_H
#define SHITCRAFT_GAME_H

#include "SDL.h"
#include "glad/gl.h"

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

    // Callbacks

    void handleEvent(const SDL_Event &event);

    static void processGlDebugMessage(
            GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam);

    // Helpers

    [[noreturn]]
    static void die(const char *msg);

    // Data

    SDL_Window *window = nullptr;
    SDL_GLContext glContext = nullptr;
};

#endif //SHITCRAFT_GAME_H
