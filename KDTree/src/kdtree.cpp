#include "kdtree.h"
#include <limits>
#include <utility>

Point::Point(std::vector<double> coordinates) : coords(std::move(coordinates)) {}

KDNode::KDNode(Point p) : point(std::move(p)), left(nullptr), right(nullptr) {}

KDTree::KDTree(int dims) : dimensions(dims), root(nullptr) {}

KDNode* KDTree::insertUtil(KDNode* node, Point point, int depth) const {
    if (node == nullptr)
        return new KDNode(point);

    KDNode* current = node;
    KDNode* parent = nullptr;
    int dim = depth % dimensions;

    // Traverse the tree to find the appropriate position for insertion
    while (current != nullptr) {
        parent = current;
        if (point.coords[dim] < current->point.coords[dim])
            current = current->left;
        else
            current = current->right;
        depth++;
        dim = depth % dimensions;
    }

    // Create the new node and attach it to the tree
    KDNode* newNode = new KDNode(point);
    if (point.coords[dim] < parent->point.coords[dim])
        parent->left = newNode;
    else
        parent->right = newNode;

    return node;
}


KDNode* KDTree::findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth) {
    if (node == nullptr)
        return best;

    double dist = distance(node->point, target);
    if (dist < bestDist) {
        bestDist = dist;
        best = node;
    }

    int dim = depth % dimensions;
    KDNode* nearerSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->left : node->right;
    KDNode* furtherSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->right : node->left;

    best = findNearestNeighborUtil(nearerSubtree, target, best, bestDist, depth + 1);

    if (std::abs(target.coords[dim] - node->point.coords[dim]) < bestDist)
        best = findNearestNeighborUtil(furtherSubtree, target, best, bestDist, depth + 1);

    return best;
}

double KDTree::distance(Point p1, Point p2) const {
    double dist = 0.0;
    for (int i = 0; i < dimensions; ++i)
        dist += (p1.coords[i] - p2.coords[i]) * (p1.coords[i] - p2.coords[i]);
    return std::sqrt(dist);
}

void KDTree::insert(Point point) {
    root = insertUtil(root, std::move(point), 0);
}

Point KDTree::findNearestNeighbor(Point target) {
    KDNode* nearest = nullptr;
    double bestDist = std::numeric_limits<double>::max();
    nearest = findNearestNeighborUtil(root, std::move(target), nearest, bestDist, 0);
    return nearest->point;
}
