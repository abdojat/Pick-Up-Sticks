#include "Board.h"
#include <filesystem>
#include <iostream> 

Board::Board()
    : m_clickCounter(0), m_level(1), m_timeRemaining(60.0f), m_sticksLeft(15), m_sticksPickable(1), m_gameOver(false), m_showHints(false), m_flashSticksAbove(false), m_difficulty(1)
{
    initGameUI(); 
    m_background.setSize(sf::Vector2f(1200, 900));
    m_background.setFillColor(sf::Color(200, 200, 200)); // Light gray background

    m_uiPanel.setSize(sf::Vector2f(200, 900));
    m_uiPanel.setFillColor(sf::Color(220, 220, 220)); // Slightly darker gray for UI panel
    m_uiPanel.setPosition(1200, 0);

    m_hintButton.setSize(sf::Vector2f(180, 40));
    m_hintButton.setFillColor(sf::Color(100, 100, 100)); // Dark gray
    m_hintButton.setPosition(1210, 850);

    if (!m_font.loadFromFile("resources/FONT.ttf"))
    {
        std::cerr << "Error loading font" << std::endl; 
    }

    m_levelText.setFont(m_font);
    m_levelText.setCharacterSize(24);
    m_levelText.setFillColor(sf::Color::Black);
    m_levelText.setPosition(1210, 10);

    m_clickText.setFont(m_font);
    m_clickText.setCharacterSize(24);
    m_clickText.setFillColor(sf::Color::Black);
    m_clickText.setPosition(1210, 40);

    m_timeText.setFont(m_font);
    m_timeText.setCharacterSize(24);
    m_timeText.setFillColor(sf::Color::Black);
    m_timeText.setPosition(1210, 70);

    m_sticksLeftText.setFont(m_font);
    m_sticksLeftText.setCharacterSize(24);
    m_sticksLeftText.setFillColor(sf::Color::Black);
    m_sticksLeftText.setPosition(1210, 100);

    m_sticksPickableText.setFont(m_font);
    m_sticksPickableText.setCharacterSize(24);
    m_sticksPickableText.setFillColor(sf::Color::Black);
    m_sticksPickableText.setPosition(1210, 130);

    m_hintButtonText.setFont(m_font);
    m_hintButtonText.setCharacterSize(20);
    m_hintButtonText.setFillColor(sf::Color::White);
    m_hintButtonText.setPosition(1215, 855);
    m_hintButtonText.setString("Show Hints");

    m_finalScoreText.setFont(m_font);
    m_finalScoreText.setCharacterSize(30);
    m_finalScoreText.setFillColor(sf::Color::Red);
    m_finalScoreText.setPosition(600, 450);

    initGameOverScreen();
    createBoard();
}

void Board::initGameOverScreen() {
    m_restartButton.setSize(sf::Vector2f(180, 40));
    m_restartButton.setFillColor(sf::Color::Blue);
    m_restartButton.setPosition(600, 550);

    m_restartButtonText.setFont(m_font);
    m_restartButtonText.setCharacterSize(30);
    m_restartButtonText.setFillColor(sf::Color::Black);
    m_restartButtonText.setString("Restart Game");
    m_restartButtonText.setPosition(600, 550);
}

void Board::initGameUI() {
    m_saveButton.setSize(sf::Vector2f(180, 40));
    m_saveButton.setFillColor(sf::Color::Magenta);
    m_saveButton.setPosition(1210, 800); 

    m_saveButtonText.setFont(m_font);
    m_saveButtonText.setCharacterSize(20);
    m_saveButtonText.setFillColor(sf::Color::White);
    m_saveButtonText.setPosition(1215, 805);
    m_saveButtonText.setString("Save Game");
}

void Board::setRestartCallback(const std::function<void()>& callback) {
    m_restartCallback = callback;
}

void Board::setDifficulty(int difficulty) {
    m_difficulty = difficulty;
    m_sticksLeft = 10 * difficulty; // Adjust the number of sticks based on difficulty
    createBoard();
}

void Board::handleClick(const sf::Vector2f& pos) {
    if (m_gameOver) {
        handleRestartButtonClick(pos);
        return;
    }

    if (m_hintButton.getGlobalBounds().contains(pos)) {
        handleHintButtonClick();
        return;
    }

    Stick* stick = graph.getStickAtPosition(pos);
    if (stick) {
        if (!graph.isTopStick(stick)) {
            m_flashSticksAbove = true;
            m_flashSticksAboveClock.restart();
            graph.flashSticksAbove(stick);
            return;
        }

        graph.deleteStick(stick);
        graph.getTopoSort();
        m_clickCounter++;
        m_sticksLeft--;
        if (m_sticksLeft == 0) {
            nextLevel();
        }
        graph.getTopoSort();
        updateText();
    }
}

void Board::handleRestartButtonClick(const sf::Vector2f& pos) {
    if (m_restartButton.getGlobalBounds().contains(pos) && m_restartCallback) {
        m_restartCallback();
    }
}

void Board::handleHintButtonClick() {
    m_showHints = true;
    m_hintClock.restart(); // Restart the clock when the hint button is pressed
    m_sticksPickable = graph.getSourcesSize();
    for (auto& stick : graph.getSticks()) {
        if (graph.isTopStick(stick)) {
            stick->setFlash(true);
        }
    }
}
void Board::handleSaveButtonClick(const sf::Vector2f& pos) {
    if (m_saveButton.getGlobalBounds().contains(pos)) {
        saveGameState(); // Call saveGameState without filename, the method generates it
    }
}

void Board::draw(sf::RenderWindow& window) {
    window.draw(m_background);

    graph.getTopoSort();
    m_sticksPickable = graph.getSourcesSize();
    graph.draw(window);

    window.draw(m_uiPanel);
    window.draw(m_levelText);
    window.draw(m_clickText);
    window.draw(m_timeText);
    window.draw(m_sticksLeftText);
    window.draw(m_sticksPickableText);
    window.draw(m_hintButton);
    window.draw(m_hintButtonText);
    window.draw(m_saveButton); // Draw save button
    window.draw(m_saveButtonText); // Draw save button text

    if (m_gameOver) {
        window.draw(m_finalScoreText);
        window.draw(m_restartButton);
        window.draw(m_restartButtonText);
    }
}

void Board::createBoard() {
    graph = Graph();
    sf::Color colors[5] = {
        sf::Color(255, 0, 0),   // Red
        sf::Color(0, 255, 0),   // Green
        sf::Color(0, 0, 255),   // Blue
        sf::Color(255, 255, 0), // Yellow
        sf::Color(255, 165, 0)  // Orange
    };
    srand(static_cast<unsigned int>(time(0))); // Ensure different random seed each run
    for (int i = 0; i < m_sticksLeft; ++i) {
        int len = 100 + rand() % 200;
        int x = rand() % (1200 - 2 * len) + len;
        int y = rand() % (800 - 2 * len) + len;
        float angle = rand() % 360 * 3.14159265 / 180.0;
        float halfWidth = 7.5;
        float halfLength = (len + 60) / 2.0f;
        float x1, x2, x3, x4, y1, y2, y3, y4;
        x1 = -halfLength * cos(angle) - halfWidth * sin(angle) + (float)x, y1 = (float)y - halfLength * sin(angle) + halfWidth * cos(angle);
        x2 = halfLength * cos(angle) - halfWidth * sin(angle) + (float)x, y2 = (float)y + halfLength * sin(angle) + halfWidth * cos(angle);
        x3 = halfLength * cos(angle) + halfWidth * sin(angle) + (float)x, y3 = (float)y + halfLength * sin(angle) - halfWidth * cos(angle);
        x4 = -halfLength * cos(angle) + halfWidth * sin(angle) + (float)x, y4 = (float)y - halfLength * sin(angle) - halfWidth * cos(angle);
        std::vector<std::pair<float, float>> vec;
        vec.push_back({ x1,y1 });
        vec.push_back({ x2,y2 });
        vec.push_back({ x3,y3 });
        vec.push_back({ x4,y4 });
        float centroidX = 0;
        float centroidY = 0;
        for (const auto& point : vec) {
            centroidX += point.first;
            centroidY += point.second;
        }
        centroidX /= 4;
        centroidY /= 4;

        // Sort points based on angle with respect to centroid
        std::sort(vec.begin(), vec.end(), [centroidX, centroidY](const std::pair<float, float>& a, const std::pair<float, float>& b) {
            float angleA = std::atan2(a.second - centroidY, a.first - centroidX);
            float angleB = std::atan2(b.second - centroidY, b.first - centroidX);
            return angleA < angleB;
            });
        sf::Color col = colors[rand() % 5];
        Stick* newStick = new Stick(vec, col);
        graph.addStick(newStick);
    }
    m_sticksPickable = graph.getSourcesSize();
    updateText();
}

void Board::updateTime(float elapsedTime) {
    if (!m_gameOver) {
        m_timeRemaining -= elapsedTime;
        if (m_timeRemaining < 0) {
            m_timeRemaining = 0;
            m_gameOver = true;
            m_finalScoreText.setString("Game Over!\nFinal Score: " + std::to_string(m_clickCounter));
        }
        graph.getTopoSort();
        m_sticksPickable = graph.getSourcesSize();
        updateText();
    }
    graph.getTopoSort();
    m_sticksPickable = graph.getSourcesSize();
    // Stop flashing after a short time
    if (m_showHints) {
        sf::Time elapsed = m_hintClock.getElapsedTime();
        if (elapsed.asSeconds() > 0.5f) {
            m_showHints = false;
            for (auto& stick : graph.getSticks()) {
                stick->setFlash(false);
            }
        }
    }
    if (m_flashSticksAbove) {
        sf::Time elapsed = m_flashSticksAboveClock.getElapsedTime();
        if (elapsed.asSeconds() > 0.5f) {
            m_flashSticksAbove = false;
            for (auto& stick : graph.getSticks()) {
                stick->setFlash(false);
            }
        }
    }
}

void Board::reset() {
    m_clickCounter = 0;
    m_level = 1;
    m_timeRemaining = 60.0f;
    m_sticksLeft = 15;
    m_sticksPickable = 1;
    m_gameOver = false;
    m_showHints = false;
    m_flashSticksAbove = false;
    m_difficulty = 1;

    m_background.setSize(sf::Vector2f(1200, 900));
    m_background.setFillColor(sf::Color(200, 200, 200)); // Light gray background

    m_uiPanel.setSize(sf::Vector2f(200, 900));
    m_uiPanel.setFillColor(sf::Color(220, 220, 220)); // Slightly darker gray for UI panel
    m_uiPanel.setPosition(1200, 0);

    m_hintButton.setSize(sf::Vector2f(180, 40));
    m_hintButton.setFillColor(sf::Color(100, 100, 100)); // Dark gray
    m_hintButton.setPosition(1210, 850);

    m_levelText.setFont(m_font);
    m_levelText.setCharacterSize(24);
    m_levelText.setFillColor(sf::Color::Black);
    m_levelText.setPosition(1210, 10);

    m_clickText.setFont(m_font);
    m_clickText.setCharacterSize(24);
    m_clickText.setFillColor(sf::Color::Black);
    m_clickText.setPosition(1210, 40);

    m_timeText.setFont(m_font);
    m_timeText.setCharacterSize(24);
    m_timeText.setFillColor(sf::Color::Black);
    m_timeText.setPosition(1210, 70);

    m_sticksLeftText.setFont(m_font);
    m_sticksLeftText.setCharacterSize(24);
    m_sticksLeftText.setFillColor(sf::Color::Black);
    m_sticksLeftText.setPosition(1210, 100);

    m_sticksPickableText.setFont(m_font);
    m_sticksPickableText.setCharacterSize(24);
    m_sticksPickableText.setFillColor(sf::Color::Black);
    m_sticksPickableText.setPosition(1210, 130);

    m_hintButtonText.setFont(m_font);
    m_hintButtonText.setCharacterSize(20);
    m_hintButtonText.setFillColor(sf::Color::White);
    m_hintButtonText.setPosition(1215, 855);
    m_hintButtonText.setString("Show Hints");

    m_finalScoreText.setFont(m_font);
    m_finalScoreText.setCharacterSize(30);
    m_finalScoreText.setFillColor(sf::Color::Red);
    m_finalScoreText.setPosition(600, 450);

    initGameOverScreen();
    createBoard();
    updateText();
}


std::vector<std::string> Board::listSavedGames() {
    std::vector<std::string> savedGames;
    std::filesystem::create_directory("saved_games");
    for (const auto& entry : std::filesystem::directory_iterator("saved_games")) {
        savedGames.push_back(entry.path().string());
    }
    return savedGames;
}

void Board::updateText() {
    m_levelText.setString("Level: " + std::to_string(m_level));
    m_clickText.setString("Clicks: " + std::to_string(m_clickCounter));
    m_timeText.setString("Time: " + std::to_string(static_cast<int>(m_timeRemaining)) + " s");
    m_sticksLeftText.setString("Sticks Left: " + std::to_string(m_sticksLeft));
    m_sticksPickableText.setString("Pickable Sticks: " + std::to_string(m_sticksPickable));
}

void Board::nextLevel() {
    m_level++;
    m_sticksLeft = m_level * 15;
    createBoard();
}

bool Board::isGameOver() const {
    return m_gameOver;
}

void Board::showFinalScore(sf::RenderWindow& window) {
    window.clear(sf::Color::White);
    window.draw(m_finalScoreText);
    window.draw(m_restartButton);
    window.draw(m_restartButtonText);
    window.display();
}
void Board::deleteGameState(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
    else {
        std::cerr << "Error: File not found: " << filename << std::endl;
    }
}
