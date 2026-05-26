#include "rectangle.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

Rectangle::Rectangle(const Point& leftBottom, const Point& rightTop) 
    : leftBottom_(leftBottom), rightTop_(rightTop) {
    if (leftBottom_.x > rightTop_.x) {
        std::swap(leftBottom_.x, rightTop_.x);
    }

    if (leftBottom_.y > rightTop_.y) {
        std::swap(leftBottom_.y, rightTop_.y);
    }

    if (leftBottom_.x == rightTop_.x || leftBottom_.y == rightTop_.y) {
        throw std::invalid_argument("Rectangle area must be positive");
    }
}

double Rectangle::getArea() const {
    double width = rightTop_.x - leftBottom_.x;
    double height = rightTop_.y - leftBottom_.y;
    
    return width * height; 
}

Point Rectangle::getCenter() const {
    double centerX = (leftBottom_.x + rightTop_.x) / 2;
    double centerY = (leftBottom_.y + rightTop_.y) / 2;

    Point center(centerX, centerY);
    return center;
}

void Rectangle::move(double dx, double dy) {
    leftBottom_.x += dx;
    leftBottom_.y += dy;
    rightTop_.x += dx;
    rightTop_.y += dy;
}

void Rectangle::scale(double factor) {
    if (factor <= 0) {
        throw std::invalid_argument("Scale factor must be positive");
    }

    Point center = this->getCenter();    

    // x' = cx + (x - cx) * k
    // y' = cy + (y - cy) * k

    // new value   = center +     we find the difference between point and center and multiply it by factor
    leftBottom_.x = center.x + (leftBottom_.x - center.x) * factor;
    leftBottom_.y = center.y + (leftBottom_.y - center.y) * factor;

    rightTop_.x = center.x + (rightTop_.x - center.x) * factor;
    rightTop_.y = center.y + (rightTop_.y - center.y) * factor;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::print() const {
    Point center = getCenter();

    std::cout << std::fixed << std::setprecision(2)
              << "[" << getName()
              << ", (" << center.x << ", " << center.y << ")"
              << ", " << getArea()
              << "]";
}

void Rectangle::getBounds(
    double& minX, double& minY,
    double& maxX, double& maxY
) const {
    minX = leftBottom_.x;
    minY = leftBottom_.y;
    maxX = rightTop_.x;
    maxY = rightTop_.y;
}
