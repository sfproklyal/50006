#include "composite_shape.h"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (!shape) return;

    if (dynamic_cast<CompositeShape*>(shape.get())) {
        throw std::invalid_argument("CompositeShape can contain only simple shapes");
    }

    shapes_.push_back(std::move(shape));
}

const std::vector<std::unique_ptr<Shape>>& CompositeShape::getShapes() const {
    return shapes_;
}

double CompositeShape::getArea() const {
    double totalArea = 0;
    for (size_t i = 0; i < shapes_.size(); i++) {
        totalArea += shapes_.at(i)->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const {
        double minX = 0.0;
        double minY = 0.0;
        double maxX = 0.0;
        double maxY = 0.0;

        getBounds(minX, minY, maxX, maxY);
        Point center((minX + maxX) / 2.0, (minY + maxY) / 2.0);
        return center;
}



void CompositeShape::move(double dx, double dy) {
    for (size_t i = 0; i < shapes_.size(); i++) {
        shapes_.at(i)->move(dx, dy);
    }
}



void CompositeShape::scale(double factor) {
    if (factor <= 0) throw std::invalid_argument("Scale factor must be greater than 0");

    if (shapes_.empty()) return;

    Point compositeCenter(this->getCenter());

    for (size_t i = 0; i < shapes_.size(); i++) {
        Point oldCenter = shapes_.at(i)->getCenter();

        double oldDx = oldCenter.x - compositeCenter.x;
        double oldDy = oldCenter.y - compositeCenter.y;

        double newDx = oldDx * factor;
        double newDy = oldDy * factor;

        double moveDx = newDx - oldDx;
        double moveDy = newDy - oldDy;

        shapes_.at(i)->move(moveDx, moveDy);
        shapes_.at(i)->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return  "COMPOSITE";
}

void CompositeShape::print() const {
    Point center = getCenter();

    std::cout << std::fixed << std::setprecision(2)
              << "[" << getName()
              << ", (" << center.x << ", " << center.y << ")"
              << ", " << getArea()
              << ":\n";

    for (size_t i = 0; i < shapes_.size(); i++) {
        Point shapeCenter = shapes_.at(i)->getCenter();

        std::cout << shapes_.at(i)->getName()
                  << ", (" << shapeCenter.x << ", " << shapeCenter.y << ")"
                  << ", " << shapes_.at(i)->getArea();

        if (i + 1 < shapes_.size()) {
            std::cout << ",";
        }

        std::cout << '\n';
    }

    std::cout << "]";
}

void CompositeShape::getBounds(
    double& minX, double& minY,
    double& maxX, double& maxY
) const {

    if (shapes_.empty()) {
        minX = 0.0;
        minY = 0.0;
        maxX = 0.0;
        maxY = 0.0;
        return;
    }

    shapes_.at(0)->getBounds(minX, minY, maxX, maxY);

    for (size_t i = 0; i < shapes_.size(); i++) {

        double shapeMinX = 0.0;
        double shapeMinY = 0.0;
        double shapeMaxX = 0.0;
        double shapeMaxY = 0.0;

        shapes_.at(i)->getBounds(shapeMinX, shapeMinY, shapeMaxX, shapeMaxY);

        minX = std::min(minX, shapeMinX);
        minY = std::min(minY, shapeMinY);
        maxX = std::max(maxX, shapeMaxX);
        maxY = std::max(maxY, shapeMaxY);

    }

}
