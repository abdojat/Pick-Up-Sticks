#ifndef SAVE_STATE_H
#define SAVE_STATE_H
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include "board.h"
#include "json.hpp"
using json = nlohmann::json;

struct StickState {
    std::vector<std::pair<float, float>> points;
   vector<int> color;
    bool flash;
    int degree;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(StickState, points, color, flash, degree)
};

struct SaveState {
    int level;
    int clickCounter;
    float timeRemaining;
    int sticksLeft;
    std::vector<StickState> sticks;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveState, level, clickCounter, timeRemaining, sticksLeft, sticks)
};

#endif
