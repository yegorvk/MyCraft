#include "Game.h"
#include "config.h"
#include "spdlog/spdlog.h"

constexpr int WINDOW_WIDTH = 960;
constexpr int WINDOW_HEIGHT = 720;

int main(int, char **) {
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    Game("ShitCraft " VERSION_STR, WINDOW_WIDTH, WINDOW_HEIGHT, false).run();

    return 0;
}