#include "src/kdtree.h"
#include <iostream>

int main() {
    KDTree tree(2); // 2 dimensions

    // Insert some points
    tree.insert(Point({2, 3}));
    tree.insert(Point({5, 4}));
    tree.insert(Point({9, 6}));
    tree.insert(Point({4, 7}));
    tree.insert(Point({8, 1}));
    tree.insert(Point({7, 2}));

    // Find nearest neighbor to a target point
    Point target({6, 3});
    Point nearest = tree.findNearestNeighbor(target);
    std::cout << "Nearest neighbor to (" << target.coords[0] << ", " << target.coords[1] << ") is (" << nearest.coords[0] << ", " << nearest.coords[1] << ")\n";

    return 0;
}