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

class Grid {
    /* The grid is specially for geometry Mesh */
public:
    using Mesh_Indices = std::vector<int>; // The index for mesh. Use (*3) to get the real index
    typedef struct Entry {
        bool isOverlapWith(const AABB& aabb) const {
            return this->aabb.isOverlapWith(aabb);
        }
        bool isIntersect(const Ray& ray) const {
            return this->aabb.intersect(ray);
        }
        AABB aabb;
        Mesh_Indices indices;
    } Entry;
    Grid(): resolution(1) {
        entries.resize(resolution * resolution * resolution);
    }
    Grid(int res): resolution(res) {
        entries.resize(resolution * resolution * resolution);
    }

    Entry& getEntry(int i, int j, int k) {
        return entries[i * resolution * resolution + j * resolution + k];
    }

    void init(const AABB& total_aabb) {
        printf("Init Grid\n");
        aabb = total_aabb;
        Vec3f size = total_aabb.getMax() - total_aabb.getMin();
        Vec3f cell_size = size / resolution;
        for (int i = 0; i < resolution; i++) {
            for (int j = 0; j < resolution; j++) {
                for (int k = 0; k < resolution; k++) {
                    Vec3f min = total_aabb.getMin() + Vec3f(i, j, k).cwiseProduct(cell_size);
                    Vec3f max = min + cell_size;
                    entries[i * resolution * resolution + j * resolution + k] = Entry{AABB(min, max), Mesh_Indices()};
                }
            }
        }
    }


    void addIndex(int i, int j, int k, int index) {
        entries[i * resolution * resolution + j * resolution + k].indices.push_back(index);
    }

    // Total AABB
    AABB aabb;
    // For Grid
    int resolution;
    std::vector<Entry> entries;
};


#endif // ACCEL_HPP_