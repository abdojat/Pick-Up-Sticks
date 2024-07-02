#ifndef STICK_H
#define STICK_H
#include "convex.h"
#include <SFML/Graphics.hpp>
using namespace std;

class Stick
{
public:
    Stick(vector<pair<float, float>>, sf::Color col);
    void increaseDegree();
    void decreaseDegree();
    int getDegree();
    void draw(sf::RenderWindow& window);
    bool contains(const sf::Vector2f& point) const;
    bool intersects(const Stick* other) const;
    void setFlash(bool flash);
    const sf::ConvexShape& getShape() const;
    vector<std::pair<float, float>> getPoints()  { return conv.getPoints(); } 
    sf::Color getColor() const { return m_color; }
    bool getFlash() const { return m_flash; }
    int getDegree() const { return m_degree; }
    void setDegree(int d) { m_degree = d; }
private:
    Convex conv;
    sf::Color m_color;
    sf::ConvexShape m_shape;
    bool m_flash;
    int m_degree;
    sf::Clock m_flashClock;
};

#endif 
