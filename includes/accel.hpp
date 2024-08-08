#ifndef ACCEL_HPP_
#define ACCEL_HPP_

#include "utils.hpp"
#include "camera.hpp"
#include "interaction.hpp"

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
        // Set the num of decimal places: 2
        os << std::fixed << std::setprecision(2);
        os << "AABB: (" << 
        aabb.xyz_min.x() << ", " << aabb.xyz_min.y() << ", " << aabb.xyz_min.z() <<
        ") -> (" << 
        aabb.xyz_max.x() << ", " << aabb.xyz_max.y() << ", " << aabb.xyz_max.z() << ")";
        return os;
    }

private:
    Vec3f xyz_min, xyz_max;
};


struct GridCell {
// Constructor
    GridCell(): aabb(Vec3f(0,0,0), Vec3f(1,1,1)) {};
    GridCell(const Vec3f& min, const Vec3f& max): aabb(min, max) {}

    void add_object(std::shared_ptr<Geometry> object) {
        objects.push_back(object);
    }

    // The Grid Cell
    AABB aabb;
    // Objects
    std::vector<std::shared_ptr<Geometry>> objects;
};

class OccupancyGrid {
public:
    OccupancyGrid(): grid_resolution(4, 4, 4) {
        occupancy_grid.resize(grid_resolution.x() * grid_resolution.y() * grid_resolution.z());
    }
    OccupancyGrid(int resolution): grid_resolution(resolution, resolution, resolution) {
        occupancy_grid.resize(grid_resolution.x() * grid_resolution.y() * grid_resolution.z());
    }
    OccupancyGrid(int res_x, int res_y, int res_z): grid_resolution(res_x, res_y, res_z) {
        occupancy_grid.resize(grid_resolution.x() * grid_resolution.y() * grid_resolution.z());
    }
    OccupancyGrid(const Vec3i& resolution): grid_resolution(resolution) {
        occupancy_grid.resize(grid_resolution.x() * grid_resolution.y() * grid_resolution.z());
    }


    bool intersect(const Ray& ray, Interaction& interaction) const;


    std::vector<GridCell> occupancy_grid;
    Vec3i grid_resolution;
    AABB aabb;
};

#endif // ACCEL_HPP_