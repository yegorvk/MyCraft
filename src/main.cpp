#include "Game.h"
#include "config.h"
#include "spdlog/spdlog.h"
#include "core/Executor.h"

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

int main(int argv, char **args) {
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    Game("ShitCraft " VERSION_STR, WINDOW_WIDTH, WINDOW_HEIGHT, false).run();

    return 0;
}