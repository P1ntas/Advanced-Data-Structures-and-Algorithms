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
        std::vector<std::string> keys = Song().keys;
        for (int i = 0; i < keys.size(); ++i)
        {
            numeric_data[keys[i]] = coords[i];
        }
        this->song = Song(numeric_data, "DEFAULT", "DEFAULT", "DEFAULT", "DEFAULT");
    }

    Point(Song &song)
    {
        this->song = song;
        this->coords = song.getCoordinates();
    }
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
    int getDimensions() const;
};

#endif //KDTREE_KDTREE_H
