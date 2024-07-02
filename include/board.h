#ifndef BOARD_H
#define BOARD_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "Graph.h"
#include "saveState.h"

class Board {
public:
    Board();

    void setDifficulty(int difficulty);
    void handleClick(const sf::Vector2f& pos);
    void updateTime(float elapsedTime);
    void draw(sf::RenderWindow& window);
    bool isGameOver() const;
    void showFinalScore(sf::RenderWindow& window);
    void setRestartCallback(const std::function<void()>& callback);
    void reset();
    void handleSaveButtonClick(const sf::Vector2f& pos);
    void saveGameState();
    void loadGameState( std::string& filename);
    void deleteGameState(const std::string& filename);

    std::string generateSaveFilename();
    std::vector<std::string> listSavedGames();

private:
    int m_clickCounter;
    int m_level;
    float m_timeRemaining;
    int m_sticksLeft;
    int m_sticksPickable;
    bool m_gameOver;
    bool m_showHints;
    bool m_flashSticksAbove;
    int m_difficulty;

    sf::RectangleShape m_background;
    sf::RectangleShape m_uiPanel;
    sf::RectangleShape m_hintButton;
    sf::RectangleShape m_restartButton;
    sf::Font m_font;
    sf::Text m_levelText;
    sf::Text m_clickText;
    sf::Text m_timeText;
    sf::Text m_sticksLeftText;
    sf::Text m_sticksPickableText;
    sf::Text m_hintButtonText;
    sf::Text m_finalScoreText;
    sf::Text m_restartButtonText;

    Graph graph;

    sf::Clock m_flashSticksAboveClock;
    sf::Clock m_hintClock;
    std::function<void()> m_restartCallback;
    sf::RectangleShape m_saveButton;
    sf::Text m_saveButtonText;

    void initGameUI();
    void initGameOverScreen();
    void handleRestartButtonClick(const sf::Vector2f& pos);
    void handleHintButtonClick();
    void createBoard();
    void updateText();
    void nextLevel();
};

#endif
