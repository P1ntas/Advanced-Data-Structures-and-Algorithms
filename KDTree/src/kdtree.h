#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <iostream>
#include <vector>
#include <cmath>
#include "song.h"

struct Point {
    Song song;
    std::vector<double> coords;
    Point(std::vector<double> coordinates) : coords(std::move(coordinates)) {
        // Each key in the map is a dimension in the KDTree
        // Create a song object with the given coordinates
        std::map<std::string, double> numeric_data;
        const std::vector<std::string> keys = Song().get_keys();
        for (int i = 0; i < keys.size(); ++i)
        {
            numeric_data[keys[i]] = coords[i];
        }
    }

    Point(Song &song)
    {
        this->song = song;
        this->coords = song.get_coordinates();
    }
};

class KDNode {
public:
    Point point;
    KDNode* left;
    KDNode* right;

    explicit KDNode(Point p);
};

class KDTree {
private:
    const int dimensions;
    KDNode* root;

    KDNode* insertUtil(KDNode* node, Point point, int depth) const;
    
    KDNode* buildTree(std::vector<Point> points, int currentDimension);

    KDNode* findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth);
    double distance(const Point &p1, const Point &p2) const;
    void clear(KDNode* node);

public:
    explicit KDTree(int dims);
    KDTree(int dims, std::vector<Point> points);
    ~KDTree();

    void insert(Point point);
    Point findNearestNeighbor(Point target);
    std::vector<Point> findKNearestNeighbors(Point target, int k) const;
    int getDimensions() const;
};

#endif //KDTREE_KDTREE_H
