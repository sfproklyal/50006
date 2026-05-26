#include "point.h"
#include "rectangle.h"
#include "rhombus.h"
#include "shape.h"
#include "composite_shape.h"

#include <iostream>
#include <vector>
#include <memory>

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(
        std::make_unique<Rectangle>(
            Point(0.0, 0.0), Point(4.0, 2.0)
        )
    );

    shapes.push_back(
        std::make_unique<Rectangle>(
            Point(3.0, 3.0), Point(9.0, 9.0)
        )
    );

    shapes.push_back(
        std::make_unique<Rhombus>(
            Point(0.0, 2.0), 3.0, 4.5
        )
    );

    shapes.push_back(
        std::make_unique<Rhombus>(
            Point(5.0, 1.0), 2.0, 3.5
        )
    );

    std::unique_ptr<CompositeShape> composite = std::make_unique<CompositeShape>();
    composite->addShape(
        std::make_unique<Rectangle>(
            Point(-2.0, -1.0), Point(1.0, 1.0)
        )
    );

    composite->addShape(
        std::make_unique<Rhombus>(
            Point(3.0, -2.0), 2.0, 4.0
        )
    );

    shapes.push_back(std::move(composite));

    for (size_t i = 0; i < shapes.size(); i++) {
        shapes.at(i)->print();
        std::cout << '\n';
    }

    for (size_t i = 0; i < shapes.size(); i++) {
        shapes.at(i)->scale(2.0);
    }

    std::cout << '\n';

    for (size_t i = 0; i < shapes.size(); i++) {
        shapes.at(i)->print();
        std::cout << '\n';
    }
    


    return 0;
}
