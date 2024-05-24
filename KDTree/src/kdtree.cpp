#include "kdtree.h"
#include <limits>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>

Point::Point(Song song_data) : song(std::move(song_data)), coords(song.getCoordinates()) {}

KDNode::KDNode(Point p) : point(std::move(p)), left(nullptr), right(nullptr) {}

KDTree::KDTree(int dims) : dimensions(dims), root(nullptr) {}

KDNode* KDTree::insertUtil(KDNode* node, Point point, int depth) const {
    if (node == nullptr)
        return new KDNode(point);

    KDNode* current = node;
    KDNode* parent = nullptr;
    int dim = depth % dimensions;

    while (current != nullptr) {
        parent = current;
        if (point.coords[dim] < current->point.coords[dim])
            current = current->left;
        else
            current = current->right;
        depth++;
        dim = depth % dimensions;
    }

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

double KDTree::distance(const Point& p1, const Point& p2) const {
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

std::vector<Point> KDTree::findKNearestNeighbors(Point target, int k) {
    std::priority_queue<std::pair<double, KDNode*>> pq;

    auto compareDistances = [&](KDNode* a, KDNode* b) {
        return distance(a->point, target) < distance(b->point, target);
    };

    auto insertIntoQueue = [&](KDNode* node) {
        double dist = distance(node->point, target);
        pq.push({dist, node});
        if (pq.size() > k) pq.pop(); // Keep size <= k
    };

    std::function<void(KDNode*, int)> traverse = [&](KDNode* node, int depth) {
        if (node == nullptr) return;

        int dim = depth % dimensions;
        KDNode* nearerSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->left : node->right;
        KDNode* furtherSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->right : node->left;

        insertIntoQueue(node);

        traverse(nearerSubtree, depth + 1);

        if (std::abs(target.coords[dim] - node->point.coords[dim]) < pq.top().first || pq.size() < k)
            traverse(furtherSubtree, depth + 1);
    };

    traverse(root, 0);

    std::vector<Point> nearestNeighbors;
    while (!pq.empty()) {
        nearestNeighbors.push_back(pq.top().second->point);
        pq.pop();
    }

    std::reverse(nearestNeighbors.begin(), nearestNeighbors.end());

    return nearestNeighbors;
}
