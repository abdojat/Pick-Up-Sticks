#include "Controller.h"

Controller::Controller(sf::RenderWindow& window)
    : m_window(window), m_board(), m_isRunning(true), m_showWelcomeScreen(true), m_difficulty(1)
{
    initWelcomeScreen();
    if (!m_font.loadFromFile("resources/FONT.ttf")) {

    }
    m_board.setRestartCallback([this]() { this->showWelcomeScreen(); });
}


void Controller::initWelcomeScreen() {
    if (!m_font.loadFromFile("resources/FONT.ttf")) {
    }

    m_welcomeText.setFont(m_font);
    m_welcomeText.setCharacterSize(50);
    m_welcomeText.setFillColor(sf::Color::Black);
    m_welcomeText.setString("Welcome to Pick Up Sticks!\nSelect Difficulty and Click Start");
    m_welcomeText.setPosition(200, 200);

    m_easyButtonText.setFont(m_font);
    m_easyButtonText.setCharacterSize(30);
    m_easyButtonText.setFillColor(sf::Color::Black);
    m_easyButtonText.setString("Easy");
    m_easyButtonText.setPosition(300, 400);
    m_easyButton.setSize(sf::Vector2f(180, 40));
    m_easyButton.setFillColor(sf::Color::Green);
    m_easyButton.setPosition(300, 400);

    // Medium button
    m_mediumButtonText.setFont(m_font);
    m_mediumButtonText.setCharacterSize(30);
    m_mediumButtonText.setFillColor(sf::Color::Black);
    m_mediumButtonText.setString("Medium");
    m_mediumButtonText.setPosition(300, 450);
    m_mediumButton.setSize(sf::Vector2f(180, 40));
    m_mediumButton.setFillColor(sf::Color::Yellow);
    m_mediumButton.setPosition(300, 450);

    // Hard button
    m_hardButtonText.setFont(m_font);
    m_hardButtonText.setCharacterSize(30);
    m_hardButtonText.setFillColor(sf::Color::Black);
    m_hardButtonText.setString("Hard");
    m_hardButtonText.setPosition(300, 500);
    m_hardButton.setSize(sf::Vector2f(180, 40));
    m_hardButton.setFillColor(sf::Color::Red);
    m_hardButton.setPosition(300, 500);

    // Start button
    m_startButtonText.setFont(m_font);
    m_startButtonText.setCharacterSize(30);
    m_startButtonText.setFillColor(sf::Color::Black);
    m_startButtonText.setString("Start Game");
    m_startButtonText.setPosition(600, 600);
    m_startButton.setSize(sf::Vector2f(180, 40));
    m_startButton.setFillColor(sf::Color::Blue);
    m_startButton.setPosition(600, 600);
    
    // Load button
    m_loadButtonText.setFont(m_font);
    m_loadButtonText.setCharacterSize(30);
    m_loadButtonText.setFillColor(sf::Color::Black);
    m_loadButtonText.setString("Load Game");
    m_loadButtonText.setPosition(600, 750);
    m_loadButton.setSize(sf::Vector2f(180, 40));
    m_loadButton.setFillColor(sf::Color::Cyan);
    m_loadButton.setPosition(600, 750);

    updateButtonOutlines();
}



void Controller::handleWelcomeScreenClick(const sf::Vector2f& pos) {
    if (m_easyButton.getGlobalBounds().contains(pos)) {
        m_difficulty = 1;
    }
    else if (m_mediumButton.getGlobalBounds().contains(pos)) {
        m_difficulty = 2;
    }
    else if (m_hardButton.getGlobalBounds().contains(pos)) {
        m_difficulty = 3;
    }
    else if (m_startButton.getGlobalBounds().contains(pos)) {
        m_showWelcomeScreen = false;
        m_board.setDifficulty(m_difficulty);
    }
    else if (m_loadButton.getGlobalBounds().contains(pos)) {
        showLoadableGames();
    }
    updateButtonOutlines();
}

void Controller::renderWelcomeScreen() {
    m_window.clear(sf::Color::White);

    m_window.draw(m_welcomeText);

    m_window.draw(m_easyButton);
    m_window.draw(m_easyButtonText);

    m_window.draw(m_mediumButton);
    m_window.draw(m_mediumButtonText);

    m_window.draw(m_hardButton);
    m_window.draw(m_hardButtonText);

    m_window.draw(m_startButton);
    m_window.draw(m_startButtonText);

    m_window.draw(m_loadButton);
    m_window.draw(m_loadButtonText);

    m_window.display();
}



void Controller::updateButtonOutlines() {
    m_easyButton.setOutlineThickness(m_difficulty == 1 ? 3 : 0);
    m_easyButton.setOutlineColor(sf::Color::Black);

    m_mediumButton.setOutlineThickness(m_difficulty == 2 ? 3 : 0);
    m_mediumButton.setOutlineColor(sf::Color::Black);

    m_hardButton.setOutlineThickness(m_difficulty == 3 ? 3 : 0);
    m_hardButton.setOutlineColor(sf::Color::Black);
}


void Controller::update(float deltaTime) {
    m_board.updateTime(deltaTime);
}

void Controller::render() {
    if (m_board.isGameOver()) {
        m_board.showFinalScore(m_window);
    }
    else {
        m_window.clear(sf::Color::White);
        m_board.draw(m_window);
        m_window.display();
    }
}


void Controller::showWelcomeScreen() {
    m_showWelcomeScreen = true;
    // Reset the game board state
    m_board.reset();
    //Board peo;m_board = peo;
    m_board.setRestartCallback([this]() { this->showWelcomeScreen(); });
    m_board.setDifficulty(m_difficulty); // Ensure difficulty is set
}




void Controller::showLoadableGames() {
    m_savedGames = m_board.listSavedGames();
    m_savedGameButtons.clear();
    m_savedGameTexts.clear();
    m_deleteButtons.clear();
    m_deleteButtonTexts.clear();

    for (size_t i = 0; i < m_savedGames.size(); ++i) {
        // Create load button
        sf::RectangleShape loadButton(sf::Vector2f(500, 50));
        loadButton.setPosition(100, 200 + i * 60);
        loadButton.setFillColor(sf::Color::Cyan);
        m_savedGameButtons.push_back(loadButton);

        // Create load text
        sf::Text loadText;
        loadText.setFont(m_font);
        loadText.setCharacterSize(20);
        loadText.setFillColor(sf::Color::Black);
        loadText.setString(m_savedGames[i]);
        loadText.setPosition(110, 210 + i * 60); // Position text inside the button
        m_savedGameTexts.push_back(loadText);

        // Create delete button
        sf::RectangleShape deleteButton(sf::Vector2f(50, 50));
        deleteButton.setPosition(820, 200 + i * 60);
        deleteButton.setFillColor(sf::Color::Red);
        m_deleteButtons.push_back(deleteButton);

        // Create delete text
        sf::Text deleteText;
        deleteText.setFont(m_font);
        deleteText.setCharacterSize(20);
        deleteText.setFillColor(sf::Color::White);
        deleteText.setString("X");
        deleteText.setPosition(835, 210 + i * 60); // Position text inside the button
        m_deleteButtonTexts.push_back(deleteText);
    }

    m_showWelcomeScreen = false;
}

void Controller::handleLoadScreenClick(const sf::Vector2f& pos) {
    for (size_t i = 0; i < m_savedGameButtons.size(); ++i) {
        if (m_savedGameButtons[i].getGlobalBounds().contains(pos)) {
            m_board.loadGameState(m_savedGames[i]);
            m_showWelcomeScreen = false;
            m_savedGames.clear(); // Clear the list after loading
            m_savedGameButtons.clear();
            m_savedGameTexts.clear();
            m_deleteButtons.clear();
            m_deleteButtonTexts.clear();
            return;
        }
        if (m_deleteButtons[i].getGlobalBounds().contains(pos)) {
            m_board.deleteGameState(m_savedGames[i]);
            showLoadableGames(); // Refresh the loadable games screen
            return;
        }
    }
}

void Controller::renderLoadScreen() {
    m_window.clear(sf::Color::White);
    for (size_t i = 0; i < m_savedGameButtons.size(); ++i) {
        m_window.draw(m_savedGameButtons[i]);
        m_window.draw(m_savedGameTexts[i]);
        m_window.draw(m_deleteButtons[i]);
        m_window.draw(m_deleteButtonTexts[i]);
    }
    m_window.display();
}

void Controller::handleEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_isRunning = false;
            m_window.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased && m_showWelcomeScreen) {
            handleWelcomeScreenClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
        }
        else if (event.type == sf::Event::MouseButtonReleased && !m_showWelcomeScreen && !m_savedGames.empty()) {
            handleLoadScreenClick(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            sf::Vector2f pos(event.mouseButton.x, event.mouseButton.y);
            m_board.handleClick(pos);
            m_board.handleSaveButtonClick(pos); // Handle save button click during gameplay
        }
    }
}

// Add a new method to handle load screen clicks



// Update the run method to render the load screen if needed
void Controller::run() {
    while (m_isRunning) {
        float deltaTime = m_clock.restart().asSeconds();
        handleEvents();
        if (m_showWelcomeScreen) {
            renderWelcomeScreen();
        }
        else if (!m_savedGames.empty()) {
            renderLoadScreen();
        }
        else {
            update(deltaTime);
            render();
        }
    }
}