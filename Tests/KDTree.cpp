#include <iostream>
#include <vector>
#include <memory>

// For Test
struct Point {
    float x;
    float y;
};

// KD Tree
struct KDTreeNode {
    int key;
    std::shared_ptr<KDTreeNode> left;
    std::shared_ptr<KDTreeNode> right;

    int size; // Number of points in the subtree rooted at this node
    int count; // Number of points in the subtree rooted at this node with key equal to this node's key
};

int main() {
    // Test: Sample Points
    // A: (2, 3), B: (4, 7), C: (5, 4), D: (7, 2), E: (9, 6), F: (8, 1)
    std::vector<Point> points = {
        {2, 3}, {4, 7}, {5, 4}, {7, 2}, {9, 6}, {8, 1}
    };
}