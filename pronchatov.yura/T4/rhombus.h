#ifndef RHOMBUS_H
#define RHOMBUS_H

#include <string>

#include "point.h"
#include "shape.h"

class Rhombus : public Shape {
private:
    Point center_;
    double diagonalVertical_;
    double diagonalHorizontal_;

public:
    Rhombus(const Point& centerPoint,
        double diagonalVertical,
        double diagonalHorizontal
    );

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;
    std::string getName() const override;
    void print() const override;

    void getBounds(
        double& minX, double& minY,
        double& maxX, double& maxY
    ) const override;

};

#endif
