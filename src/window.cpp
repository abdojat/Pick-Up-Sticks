#include "Window.h"

Window::Window()
    : m_window(sf::VideoMode(1400, 900), "Pick Up Sticks", sf::Style::Close | sf::Style::Titlebar)
{

}

bool Window::isOpen() const
{
    return m_window.isOpen();
}

sf::RenderWindow* Window::getWindow()
{
    return &m_window;
}
