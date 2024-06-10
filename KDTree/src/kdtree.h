#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <iostream>
#include <vector>
#include <cmath>
#include "song.h"

/**
 * @brief Represents a point in a KDTree.
 * 
 * This struct holds a Song object and its corresponding coordinates in a KDTree.
 * It provides constructors to create a Point object from a vector of coordinates or from a Song object.
 */
struct Point {
    Song song; /**< The Song object associated with the point. */
    std::vector<double> coords; /**< The coordinates of the point. */

    /**
     * @brief Constructs a Point object from a vector of coordinates.
     * 
     * Each key in the map is a dimension in the KDTree.
     * It creates a Song object with the given coordinates.
     * 
     * @param coordinates The vector of coordinates.
     */
    Point(std::vector<double> coordinates) : coords(std::move(coordinates)) {
        std::map<std::string, double> numeric_data;
        const std::vector<std::string> keys = Song().get_keys();
        for (int i = 0; i < keys.size(); ++i)
        {
            numeric_data[keys[i]] = coords[i];
        }
    }

    /**
     * @brief Constructs a Point object from a Song object.
     * 
     * @param song The Song object.
     */
    Point(Song &song)
    {
        this->song = song;
        this->coords = song.get_coordinates();
    }

    /**
     * @brief Overloaded equality operator.
     * 
     * @param other The other Point object to compare with.
     * @return True if the two points are equal, false otherwise.
     */
    bool operator==(const Point &other) const {
        return song == other.song && coords == other.coords;
    }
};

/**
 * @class KDNode
 * @brief Represents a node in a KDTree.
 *
 * The KDNode class represents a node in a KDTree data structure. Each node contains a point, 
 * a left child node, and a right child node.
 */
class KDNode {
public:
    Point point;    ///< The point stored in the node.
    KDNode* left;   ///< Pointer to the left child node.
    KDNode* right;  ///< Pointer to the right child node.

    /**
     * @brief Constructs a KDNode object with the given point.
     * @param p The point to be stored in the node.
     */
    explicit KDNode(Point p);
};

/**
 * @class KDTree
 * @brief Represents a KD-Tree data structure.
 * 
 * The KDTree class is used to store and manipulate a KD-Tree, which is a binary tree
 * used to organize points in a k-dimensional space. It supports operations like insertion,
 * nearest neighbor search, and k-nearest neighbors search.
 */
class KDTree {
private:
    const int dimensions; ///< The number of dimensions of the points in the KD-Tree.
    KDNode* root; ///< The root node of the KD-Tree.

    /**
     * @brief Utility function to recursively insert a point into the KD-Tree.
     * @param node The current node being processed.
     * @param point The point to be inserted.
     * @param depth The current depth of the node in the KD-Tree.
     * @return The updated KD-Tree after inserting the point.
     */
    KDNode* insertUtil(KDNode* node, Point point, int depth) const;
    
    /**
     * @brief Utility function to build a KD-Tree from a vector of points.
     * @param points The vector of points to build the KD-Tree from.
     * @param currentDimension The current dimension being processed.
     * @return The root node of the built KD-Tree.
     */
    KDNode* buildTree(std::vector<Point> points, int currentDimension);

    /**
     * @brief Utility function to recursively find the nearest neighbor of a target point in the KD-Tree.
     * @param node The current node being processed.
     * @param target The target point to find the nearest neighbor for.
     * @param best The current best neighbor found so far.
     * @param bestDist The current best distance to the target point.
     * @param depth The current depth of the node in the KD-Tree.
     * @return The nearest neighbor of the target point.
     */
    KDNode* findNearestNeighborUtil(KDNode* node, Point target, KDNode* best, double& bestDist, int depth);

    /**
     * @brief Calculates the Euclidean distance between two points.
     * @param p1 The first point.
     * @param p2 The second point.
     * @return The Euclidean distance between the two points.
     */
    double distance(const Point &p1, const Point &p2) const;

    /**
     * @brief Utility function to recursively clear the KD-Tree.
     * @param node The current node being processed.
     */
    void clear(KDNode* node);

public:
    /**
     * @brief Constructs a KD-Tree with the specified number of dimensions.
     * @param dims The number of dimensions of the points in the KD-Tree.
     */
    explicit KDTree(int dims);

    /**
     * @brief Constructs a KD-Tree with the specified number of dimensions and a vector of points.
     * @param dims The number of dimensions of the points in the KD-Tree.
     * @param points The vector of points to build the KD-Tree from.
     */
    KDTree(int dims, std::vector<Point> points);

    /**
     * @brief Destroys the KD-Tree and frees the memory.
     */
    ~KDTree();

    /**
     * @brief Inserts a point into the KD-Tree.
     * @param point The point to be inserted.
     */
    void insert(Point point);

    /**
     * @brief Finds the nearest neighbor of a target point in the KD-Tree.
     * @param target The target point to find the nearest neighbor for.
     * @return The nearest neighbor of the target point.
     */
    Point findNearestNeighbor(Point target);

    /**
     * @brief Finds the k-nearest neighbors of a target point in the KD-Tree.
     * @param target The target point to find the k-nearest neighbors for.
     * @param k The number of nearest neighbors to find.
     * @return A vector containing the k-nearest neighbors of the target point.
     */
    std::vector<Point> findKNearestNeighbors(Point target, int k) const;

    /**
     * @brief Gets the number of dimensions of the points in the KD-Tree.
     * @return The number of dimensions.
     */
    int getDimensions() const;
};

#endif //KDTREE_KDTREE_H
