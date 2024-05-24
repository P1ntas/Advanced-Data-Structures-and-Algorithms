#ifndef KDTREE_H
#define KDTREE_H

#include "song.h"
#include <vector>
#include <memory>

class Point {
public:
    Song song;
    std::vector<double> coords;
    Point(Song song_data);
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
    double distance(const Point& p1, const Point& p2) const;

public:
    KDTree(int dims);
    void insert(Point point);
    Point findNearestNeighbor(Point target);
    std::vector<Point> findKNearestNeighbors(Point target, int k);
};

#endif
