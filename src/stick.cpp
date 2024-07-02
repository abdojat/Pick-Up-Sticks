#include "Stick.h"
#include <cmath>

Stick::Stick(vector<pair<float, float>> vec, sf::Color col) : m_color(col), m_flash(false), m_degree(0), conv(vec)
{
    m_shape.setPointCount(4);
    for (int i = 0; i < 4; i++)
        m_shape.setPoint(i, sf::Vector2f(vec[i].first, vec[i].second));
    m_shape.setFillColor(m_color);
    m_shape.setOutlineThickness(3);
    m_shape.setOutlineColor(sf::Color::Black);
}

void Stick::increaseDegree() { m_degree++; }
void Stick::decreaseDegree() { m_degree--; }
int Stick::getDegree() { return m_degree; }

void Stick::draw(sf::RenderWindow& window)
{
    if (m_flash) {
        sf::Time elapsed = m_flashClock.getElapsedTime();
        if (elapsed.asMilliseconds() % 500 < 250) {
            m_shape.setFillColor(sf::Color::Black); 
        }
        else {
            m_shape.setFillColor(m_color);
        }
    }
    else {
        m_shape.setFillColor(m_color);
    }
    window.draw(m_shape);
}

bool Stick::contains(const sf::Vector2f& point) const
{
    return m_shape.getGlobalBounds().contains(point);
}
bool Stick::intersects(const Stick* other) const {
    if (other == nullptr) {
        return false; 
    }
    return conv.intersects(other->conv);
    return m_shape.getGlobalBounds().intersects(other->m_shape.getGlobalBounds());
}


void Stick::setFlash(bool flash)
{
    m_flash = flash;
    if (flash) {
        m_flashClock.restart();
    }
}

const sf::ConvexShape& Stick::getShape() const
{
    return m_shape;
}
