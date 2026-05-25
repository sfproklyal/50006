#ifndef POINT_H
#define POINT_H

struct Point {
    double x;
    double y;

    Point(double xCoordinate, double yCoordinate)
        : x(xCoordinate), y(yCoordinate) {
    }
    Point& operator=(const Point& other) {
        x = other.x;
        y = other.y;
        return *this;
    }

};

#endif
