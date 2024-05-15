#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    std::vector<double> coords;

    Point(std::vector<double> coordinates) : coords(coordinates) {}
};

class KDNode {
public:
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(Point p) : point(p), left(nullptr), right(nullptr) {}
};

class KDTree {
private:
    int dimensions;
    KDNode* root;

    KDNode* insertUtil(KDNode* node, Point point, int depth) {
        if (node == nullptr)
            return new KDNode(point);

        int dim = depth % dimensions;

        if (point.coords[dim] < node->point.coords[dim])
            node->left = insertUtil(node->left, point, depth + 1);
        else
            node->right = insertUtil(node->right, point, depth + 1);

        return node;
    }

    KDNode* findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth) {
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

    double distance(Point p1, Point p2) {
        double dist = 0.0;
        for (int i = 0; i < dimensions; ++i)
            dist += (p1.coords[i] - p2.coords[i]) * (p1.coords[i] - p2.coords[i]);
        return std::sqrt(dist);
    }

public:
    KDTree(int dims) : dimensions(dims), root(nullptr) {}

    void insert(Point point) {
        root = insertUtil(root, point, 0);
    }

    Point findNearestNeighbor(Point target) {
        KDNode* nearest = nullptr;
        double bestDist = std::numeric_limits<double>::max();
        nearest = findNearestNeighborUtil(root, target, nearest, bestDist, 0);
        return nearest->point;
    }
};

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
