#include "kdtree.h"
#include <limits>

Point::Point(std::vector<double> coordinates) : coords(coordinates) {}

KDNode::KDNode(Point p) : point(p), left(nullptr), right(nullptr) {}

KDTree::KDTree(int dims) : dimensions(dims), root(nullptr) {}

KDNode* KDTree::insertUtil(KDNode* node, Point point, int depth) {
    if (node == nullptr)
        return new KDNode(point);

    int dim = depth % dimensions;

    if (point.coords[dim] < node->point.coords[dim])
        node->left = insertUtil(node->left, point, depth + 1);
    else
        node->right = insertUtil(node->right, point, depth + 1);

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

double KDTree::distance(Point p1, Point p2) {
    double dist = 0.0;
    for (int i = 0; i < dimensions; ++i)
        dist += (p1.coords[i] - p2.coords[i]) * (p1.coords[i] - p2.coords[i]);
    return std::sqrt(dist);
}

void KDTree::insert(Point point) {
    root = insertUtil(root, point, 0);
}

Point KDTree::findNearestNeighbor(Point target) {
    KDNode* nearest = nullptr;
    double bestDist = std::numeric_limits<double>::max();
    nearest = findNearestNeighborUtil(root, target, nearest, bestDist, 0);
    return nearest->point;
}
