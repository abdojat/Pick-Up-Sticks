#include <SFML/Graphics.hpp>
#include "Controller.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Pick Up Sticks", sf::Style::Close | sf::Style::Titlebar);
    Controller game(window);
    game.run();
    return 0;
}