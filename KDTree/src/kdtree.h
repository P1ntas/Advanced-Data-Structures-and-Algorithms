#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <iostream>
#include <vector>
#include <cmath>

struct Point {
    std::vector<double> coords;
    Point(std::vector<double> coordinates);
};

class KDNode {
public:
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(Point p);
};

class KDTree {
private:
    int dimensions;
    KDNode* root;

    KDNode* insertUtil(KDNode* node, Point point, int depth) const;
    KDNode* findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth);
    double distance(const Point &p1, const Point &p2) const;

public:
    KDTree(int dims);

    void insert(Point point);
    Point findNearestNeighbor(Point target);
    std::vector<Point> findKNearestNeighbors(Point target, int k);
};

#endif //KDTREE_KDTREE_H
