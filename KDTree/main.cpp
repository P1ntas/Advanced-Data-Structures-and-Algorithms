#include "src/kdtree.h"
#include <iostream>

int main() {
    // Create a KD tree with 2 dimensions
    KDTree tree(2);

    // Insert some points into the tree
    tree.insert(Point({2, 3}));
    tree.insert(Point({5, 4}));
    tree.insert(Point({9, 6}));
    tree.insert(Point({4, 7}));
    tree.insert(Point({8, 1}));
    tree.insert(Point({7, 2}));

    // Find the nearest neighbor to a target point
    Point target({6, 3});
    Point nearest = tree.findNearestNeighbor(target);
    std::cout << "Nearest neighbor to (" << target.coords[0] << ", " << target.coords[1] << ") is (" << nearest.coords[0] << ", " << nearest.coords[1] << ")\n";

    // Find the 3 nearest neighbors to the target point
    int k = 3;
    std::vector<Point> nearestNeighbors = tree.findKNearestNeighbors(target, k);
    std::cout << "K Nearest neighbors to (" << target.coords[0] << ", " << target.coords[1] << "):\n";
    for (const auto& neighbor : nearestNeighbors) {
        std::cout << "(" << neighbor.coords[0] << ", " << neighbor.coords[1] << ")\n";
    }

    return 0;
}
