#include <iomanip>
#include <iostream>
#include <stdexcept>
#include "rhombus.h"

Rhombus::Rhombus(const Point& centerPoint, double diagonalVertical, double diagonalHorizontal)
    : center_(centerPoint),
      diagonalVertical_(diagonalVertical),
      diagonalHorizontal_(diagonalHorizontal) {
    if (diagonalHorizontal <= 0 || diagonalVertical <= 0) {
        throw std::invalid_argument("Diagonals must be positive");
    }
}

double Rhombus::getArea() const {
    return diagonalVertical_ * diagonalHorizontal_ / 2;
}

Point Rhombus::getCenter() const {
    Point center = center_;
    return center;
}

void Rhombus::move(double dx, double dy) {
    center_.x += dx;
    center_.y += dy;
}

void Rhombus::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    diagonalVertical_ *= factor;
    diagonalHorizontal_ *= factor;
}

std::string Rhombus::getName() const {
    return "RHOMBUS";
}

void Rhombus::print() const {
    Point center = getCenter();

    std::cout << std::fixed << std::setprecision(2)
              << "[" << getName()
              << ", (" << center.x << ", " << center.y << ")"
              << ", " << getArea()
              << "]";
}

void Rhombus::getBounds(
    double& minX, double& minY,
    double& maxX, double& maxY
) const {
    minX = center_.x - diagonalHorizontal_ / 2;
    minY = center_.y - diagonalVertical_ / 2;
    maxX = center_.x + diagonalHorizontal_ / 2;
    maxY = center_.y + diagonalVertical_ / 2;
}
