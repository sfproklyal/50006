#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include <memory>
#include <string>
#include <vector>

#include "shape.h"


class CompositeShape : public Shape {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;

public:
    void addShape(std::unique_ptr<Shape> shape);
    const std::vector<std::unique_ptr<Shape>>& getShapes() const;

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
