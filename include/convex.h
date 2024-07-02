#ifndef CONVEX_H
#define CONVEX_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
using namespace std;

class Convex {
public:
    Convex(const std::vector<std::pair<float, float>>&);
    std::vector<std::pair<float, float>> getPoints();
    bool intersects(const Convex&) const;

private:
    int num_points;
    std::vector<std::pair<float, float>> points;

    bool isSeparatingAxis(const std::pair<float, float>&, const std::pair<float, float>& , const Convex& ) const;
    void project(const std::pair<float, float>& , float& , float& ) const;
    void sortPoints();
};

#endif
