#ifndef ACCEL_HPP_
#define ACCEL_HPP_

#include "utils.hpp"
#include "camera.hpp"

class AABB {
public:
    void Update() {
        // Add eps for some axis that are parallel to the axis
        if (std::abs(xyz_min.x() - xyz_max.x()) < EPS) {
            xyz_min.x() -= EPS;
            xyz_max.x() += EPS;
        }
        if (std::abs(xyz_min.y() - xyz_max.y()) < EPS) {
            xyz_min.y() -= EPS;
            xyz_max.y() += EPS;
        }
        if (std::abs(xyz_min.z() - xyz_max.z()) < EPS) {
            xyz_min.z() -= EPS;
            xyz_max.z() += EPS;
        }
    }
    // Default constructor
    AABB(): xyz_min(0, 0, 0), xyz_max(0, 0, 0) {
        Update();
    }
    // Construct from min and max points
    AABB(const Vec3f& min, const Vec3f& max): xyz_min(min), xyz_max(max) {
        Update();
    }
    // Construct from a triangle
    AABB(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2) {
        xyz_min = utils::min(v0, v1, v2);
        xyz_max = utils::max(v0, v1, v2);
        Update();
    
    }
    // Construct by merging two AABBs
    AABB(const AABB& a, const AABB& b) {
        xyz_min = utils::min(a.xyz_min, b.xyz_min);
        xyz_max = utils::max(a.xyz_max, b.xyz_max);
        Update();
    
    }
    void merge_with(const AABB& aabb) {
        xyz_min = utils::min(xyz_min, aabb.xyz_min);
        xyz_max = utils::max(xyz_max, aabb.xyz_max);
        Update();
    
    }

    bool intersect(const Ray& ray) const;
    bool intersect(const Ray& ray, float* t_in, float* t_out) const;

    [[nodiscard]] Vec3f getCenter() const { return (xyz_min + xyz_max) / 2; }
    [[nodiscard]] float getSize(int dim) const { return xyz_max[dim] - xyz_min[dim]; }
    [[nodiscard]] Vec3f getMin() const { return xyz_min; }
    [[nodiscard]] Vec3f getMax() const { return xyz_max; }
    [[nodiscard]] bool isOverlapWith(const AABB& other) const {
        return (xyz_min.x() <= other.xyz_max.x() && xyz_max.x() >= other.xyz_min.x()) &&
               (xyz_min.y() <= other.xyz_max.y() && xyz_max.y() >= other.xyz_min.y()) &&
               (xyz_min.z() <= other.xyz_max.z() && xyz_max.z() >= other.xyz_min.z());
    }
    // Overload "<<" operator for printing
    friend std::ostream& operator<<(std::ostream& os, const AABB& aabb) {
        os << "AABB: " << aabb.xyz_min.transpose() << " -> " << aabb.xyz_max.transpose();
        return os;
    }

private:
    Vec3f xyz_min, xyz_max;
};


struct GridCell {
// Constructor
    GridCell(const Vec3f& min, const Vec3f& max): aabb(min, max) {}

    void add_object(std::shared_ptr<Geometry> object) {
        objects.push_back(object);
    }

    // The Grid Cell
    AABB aabb;
    // Objects
    std::vector<std::shared_ptr<Geometry>> objects;
};

#endif // ACCEL_HPP_