#include "kdtree.h"
#include <limits>
#include <utility>
#include <queue>
#include <functional>
#include <cmath>

Point::Point(std::vector<double> coordinates) : coords(std::move(coordinates)), song(nullptr) {}

Point::Point(const Song *song) : song(song) {
    this->coords = song->getCoordinates();
}
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

    // Helper function to compare distances for priority queue
    auto compareDistances = [&](KDNode* a, KDNode* b) {
        return distance(a->point, target) < distance(b->point, target);
    };

    // Helper function to insert into priority queue and keep size <= k
    auto insertIntoQueue = [&](KDNode* node) {
        double dist = distance(node->point, target);
        pq.push({dist, node});
        if (pq.size() > k) pq.pop(); // Keep size <= k
    };

    // Recursive function to traverse the tree and find nearest neighbors
    std::function<void(KDNode*, int)> traverse = [&](KDNode* node, int depth) {
        if (node == nullptr) return;

        int dim = depth % dimensions;
        KDNode* nearerSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->left : node->right;
        KDNode* furtherSubtree = (target.coords[dim] < node->point.coords[dim]) ? node->right : node->left;

        // Insert current node into priority queue
        insertIntoQueue(node);

        // Traverse nearer subtree
        traverse(nearerSubtree, depth + 1);

        // Check if we need to traverse further subtree
        if (std::abs(target.coords[dim] - node->point.coords[dim]) < pq.top().first || pq.size() < k)
            traverse(furtherSubtree, depth + 1);
    };

    // Start traversal from root
    traverse(root, 0);

    // Extract k nearest neighbors from priority queue
    std::vector<Point> nearestNeighbors;
    while (!pq.empty()) {
        nearestNeighbors.push_back(pq.top().second->point);
        pq.pop();
    }

    // Reverse the vector to get neighbors in ascending order of distance
    std::reverse(nearestNeighbors.begin(), nearestNeighbors.end());

    return nearestNeighbors;
}
