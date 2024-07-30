#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "camera.hpp"
#include "utils.hpp"
#include "interaction.hpp"
#include "material.hpp"

class Geometry {
public:
    Geometry(): material(nullptr) {}
    virtual ~Geometry() = default;

    virtual bool intersect(const Ray& ray, Interaction& interaction) const = 0;

    // Getters and Setters
    [[nodiscard]] std::shared_ptr<Material> getMaterial() const {
        return material;
    }
    void setMaterial(std::shared_ptr<Material> mat) {
        material = mat;
    }
protected:
    std::shared_ptr<Material> material;
};

class Triangle: public Geometry {
public:
    Triangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2): v0(v0), v1(v1), v2(v2) {
        normal = (v1 - v0).cross(v2 - v0).normalized();
    }

    bool intersect(const Ray& ray, Interaction& interaction) const override;
    
protected:
    Vec3f v0, v1, v2;
    Vec3f normal;
};

class Mesh: Geometry {
public:
    Mesh() = default;
    Mesh(
        const std::vector<Vec3f>& vertices,
        const std::vector<Vec3f>& normals,
        const std::vector<int>& v_indices,
        const std::vector<int>& n_indices
    ): vertices(vertices), normals(normals), v_indices(v_indices), n_indices(n_indices) {}
    
    bool intersect(const Ray& ray, Interaction& interaction) const override;

private:
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<int> v_indices;
    std::vector<int> n_indices;
};

#endif // GEOMETRY_HPP_