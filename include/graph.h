#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>
#include "Stick.h"
using namespace std;

class Graph {
public:
    Graph();
    void addStick(Stick* stick);
    void draw(sf::RenderWindow& window);
    Stick* getStickAtPosition(const sf::Vector2f& pos);
    bool isTopStick(Stick* stick) const;
    void flashSticksAbove(Stick* stick);
    vector<Stick*> getSticks();
    vector<Stick*> getSources();
    void getTopoSort();
    void deleteStick(Stick*);
    int getSourcesSize();

private:
    vector<Stick*> sticks, topoSort, sources;
    map< Stick*, vector<Stick*> > adjacencyList;
    map< Stick*, vector<Stick*> > adjacencyListT;
    map< Stick*, int> _degrees;
};

#endif
