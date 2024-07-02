#include "Graph.h"

Graph::Graph() {}
void Graph::addStick(Stick* stick) {
    if (stick == nullptr) {
        return; // Handle the error appropriately
    }
    adjacencyList[stick] = std::vector<Stick*>();
    for (auto it : topoSort) {
        if (it != nullptr && stick->intersects(it)) {
            it->increaseDegree();
            adjacencyList[stick].push_back(it);
        }
    }
    getTopoSort();
    sticks.push_back(stick);
}


void Graph::draw(sf::RenderWindow& window) {
    getTopoSort(); // Ensure topoSort is updated
    for (auto& stick : topoSort) {
        stick->draw(window);
    }
}

Stick* Graph::getStickAtPosition(const sf::Vector2f& pos) {
    getTopoSort(); // Ensure topoSort is updated
    for (int i = topoSort.size() - 1; i >= 0; --i) {
        auto it = topoSort[i];
        if (it->contains(pos)) {
            return it;
        }
    }
    return nullptr;
}

bool Graph::isTopStick(Stick* stick) const {
    return (stick->getDegree() == 0);
}

void Graph::flashSticksAbove(Stick* stick) {
    stick->setFlash(true);
    for (auto it : adjacencyListT[stick]) {
        flashSticksAbove(it);
    }
}

std::vector<Stick*> Graph::getSticks() {
    return topoSort;
}

std::vector<Stick*> Graph::getSources() {
    return sources;
}

void Graph::getTopoSort() {
    adjacencyListT.clear();
    std::queue<Stick*> q;
    sources.clear();
    for (auto it : adjacencyList)
    {
        _degrees[it.first] = it.first->getDegree();
        if (_degrees[it.first] == 0)
        {
            sources.push_back(it.first);
            q.push(it.first);
        }
    }
    topoSort.clear();
    while (!q.empty())
    {
        auto v = q.front();
        topoSort.push_back(v);
        q.pop();
        for (auto it : adjacencyList[v])
        {
            if (--_degrees[it] == 0)
            {
                adjacencyListT[it].push_back(v);
                q.push(it);
            }
        }
    }
    std::reverse(topoSort.begin(), topoSort.end()); 
}

void Graph::deleteStick(Stick* stick) {
    for (auto it : adjacencyList[stick])
    {
        it->decreaseDegree();
    }
    adjacencyList.erase(stick);
    sticks.erase(std::remove(sticks.begin(), sticks.end(), stick), sticks.end());
    delete (stick);
}

int Graph::getSourcesSize() {
    int num = 0;
    for (auto it : adjacencyList)
    {
        if (it.first->getDegree() == 0)
            num++;
    }
    return num;
}
