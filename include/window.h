#pragma once
#include <SFML/Graphics.hpp>
class Controller;

class Window
{
public:
    Window();
    bool isOpen() const;
    sf::RenderWindow* getWindow();

private:
    sf::RenderWindow m_window;
};