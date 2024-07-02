#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "Board.h"

class Controller {
public:
    Controller(sf::RenderWindow& window);
    void run();

private:
    void handleEvents();
    void handleWelcomeScreenClick(const sf::Vector2f& pos);
    void updateButtonOutlines();
    void initWelcomeScreen();
    void update(float deltaTime);
    void render();
    void showWelcomeScreen();
    void renderWelcomeScreen();

    void showLoadableGames();
    bool m_isRunning;
    bool m_showWelcomeScreen;
    int m_difficulty;
    std::vector<std::string> m_savedGames; 
    sf::Text m_savedGamesText[20];         
    void handleLoadScreenClick(const sf::Vector2f& pos); 
    void renderLoadScreen();

    std::vector<sf::RectangleShape> m_savedGameButtons; 
    std::vector<sf::Text> m_savedGameTexts; 
    std::vector<sf::RectangleShape> m_deleteButtons;
    std::vector<sf::Text> m_deleteButtonTexts;

    Board m_board;
    sf::RenderWindow& m_window;
    sf::RectangleShape m_easyButton;
    sf::RectangleShape m_mediumButton;
    sf::RectangleShape m_hardButton;
    sf::RectangleShape m_startButton;
    sf::RectangleShape m_saveButton;
    sf::Text m_saveButtonText;
    sf::RectangleShape m_loadButton;
    sf::Text m_loadButtonText;

    sf::Font m_font;
    sf::Text m_welcomeText;
    sf::Text m_easyButtonText;
    sf::Text m_mediumButtonText;
    sf::Text m_hardButtonText;
    sf::Text m_startButtonText;
    sf::RectangleShape* m_flashingButton;
    sf::Clock m_flashClock;
    sf::Clock m_clock;
};

#endif
