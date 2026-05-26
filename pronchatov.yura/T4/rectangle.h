#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <string>

#include "point.h"
#include "shape.h"

class Rectangle : public Shape {
private:
    Point leftBottom_;
    Point rightTop_;

public:
    Rectangle(const Point& leftBottom, const Point& rightTop_);

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
