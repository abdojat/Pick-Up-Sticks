#include "convex.h"
using namespace std;

struct Point
{
    int x;
    int y;
};

bool onSegment(Point p, Point q, Point r)
{
    if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
        q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
        return true;

    return false;
}

int orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  

    return (val > 0) ? 1 : 2; 
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;

}

Convex::Convex(const std::vector<std::pair<float, float>>& points)
    : points(points), num_points(points.size()) {
    sortPoints();
}

std::vector<std::pair<float, float>> Convex::getPoints()
{
    return points;
}

bool Convex::intersects(const Convex& other) const {
    for (int i = 0; i < num_points; ++i) {
        for (int j = 0; j < num_points; ++j)
        {
            Point p1, p2, q1, q2;
            p1.x = points[i].first;
            p1.y = points[i].second;
            p2.x = points[(i+1)%num_points].first;
            p2.y = points[(i+1)%num_points].second;
            q1.x = other.points[j].first;
            q1.y = other.points[j].second;
            q2.x = other.points[(j+1)%num_points].first;
            q2.y = other.points[(j+1)%num_points].second;
            if (doIntersect(p1, p2, q1, q2))
                return true;
        }
    }
    return false;
}
bool Convex::isSeparatingAxis(const std::pair<float, float>& p1, const std::pair<float, float>& p2, const Convex& other) const {
    std::pair<float, float> axis = { p2.second - p1.second, p2.first - p1.first };

    float minA, maxA, minB, maxB;
    project(axis, minA, maxA);
    other.project(axis, minB, maxB);

    return maxA < minB || maxB < minA;
}

void Convex::project(const std::pair<float, float>& axis, float& min, float& max) const {
    min = max = points[0].first * axis.first + points[0].second * axis.second;
    for (int i = 1; i < num_points; ++i) {
        float projection = points[i].first * axis.first + points[i].second * axis.second;
        if (projection < min) {
            min = projection;
        }
        if (projection > max) {
            max = projection;
        }
    }
}
void Convex::sortPoints() {
    float centroidX = 0;
    float centroidY = 0;
    for (const auto& point : points) {
        centroidX += point.first;
        centroidY += point.second;
    }
    centroidX /= num_points;
    centroidY /= num_points;

    std::sort(points.begin(), points.end(), [centroidX, centroidY](const std::pair<float, float>& a, const std::pair<float, float>& b) {
        float angleA = std::atan2(a.second - centroidY, a.first - centroidX);
        float angleB = std::atan2(b.second - centroidY, b.first - centroidX);
        return angleA < angleB;
        });
}