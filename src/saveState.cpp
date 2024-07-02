#include "saveState.h"
#include <fstream>
#include <ctime>
#include <filesystem>
#include <iostream> 

std::string Board::generateSaveFilename() {
    std::filesystem::create_directory("saved_games");

    std::time_t now = std::time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    char buf[200];
    std::strftime(buf, sizeof(buf), "saved_games/gameSave_%Y-%m-%d_%H-%M-%S.dat", &timeinfo);
    return std::string(buf);
}

void Board::saveGameState() {
    SaveState saveState;
    saveState.level = m_level;
    saveState.clickCounter = m_clickCounter;
    saveState.timeRemaining = m_timeRemaining;
    saveState.sticksLeft = m_sticksLeft;

    for (auto& stick : graph.getSticks()) {
        StickState stickState;
        stickState.points = stick->getPoints();
        auto col = stick->getColor();
        stickState.color = { col.r, col.g, col.b, col.a };
        stickState.flash = stick->getFlash();
        saveState.sticks.push_back(stickState);

    }

    json j = saveState;
    std::string filename = generateSaveFilename();
    std::ofstream ofs(filename);

    if (!ofs) {
        std::cerr << "Error opening file for saving: " << filename << std::endl;
        return;
    }
    ofs << j.dump(4); 
    ofs.close();
}

void Board::loadGameState(std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Error opening file for loading: " << filename << std::endl;
        return;
    }
    json j;
    ifs >> j;
    ifs.close();

    SaveState saveState = j.get<SaveState>();

    m_level = saveState.level;
    m_clickCounter = saveState.clickCounter;
    m_timeRemaining = saveState.timeRemaining;
    m_sticksLeft = saveState.sticksLeft;
    graph = Graph();
    for (auto& stickState : saveState.sticks) {
        sf::Color color(stickState.color[0], stickState.color[1], stickState.color[2], stickState.color[3]);
        Stick* newStick = new Stick(stickState.points, color);
        newStick->setFlash(stickState.flash);
        newStick->setDegree(stickState.degree);
        graph.addStick(newStick);
    }

    updateText();
}
