#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_

#include "camera.hpp"
#include "utils.hpp"
#include "interaction.hpp"
#include "accel.hpp"
#include "bsdf.hpp"

class Geometry {
public:
    Geometry(): material(nullptr) {}
    virtual ~Geometry() = default;

    virtual bool intersect(const Ray& ray, Interaction& interaction) const = 0;

    void buildAccel() {
        aabb.Update();
    }

    virtual std::string getType() const {
        return "Geometry";
    }

    AABB getAABB() const {
        return aabb;
    }

    // Getters and Setters
    [[nodiscard]] std::shared_ptr<BSDF> getMaterial() const {
        return material;
    }
    void setMaterial(std::shared_ptr<BSDF> mat) {
        material = mat;
    }

protected:
    std::shared_ptr<BSDF> material;
    AABB aabb;
};

class Triangle: public Geometry {
public:
    Triangle(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2): v0(v0), v1(v1), v2(v2) {
        normal = (v1 - v0).cross(v2 - v0).normalized();
        aabb = AABB(v0, v1, v2);
    }

    bool intersect(const Ray& ray, Interaction& interaction) const override;

    std::string getType() const override {
        return "Triangle";
    }
    
protected:
    Vec3f v0, v1, v2;
    Vec3f normal;
};

class Rectangle: public Geometry {
public:
    Rectangle(const Vec3f& position, const Vec2f& size, const Vec3f& normal, const Vec3f& tangent): 
    position(position), size(size), normal(normal), tangent(tangent) {
        aabb = AABB(position - Vec3f(size.x()/2, size.y()/2, 0), position + Vec3f(size.x()/2, size.y()/2, 0));
    }

    bool intersect(const Ray& ray, Interaction& interaction) const override;

    std::string getType() const override {
        return "Rectangle";
    }

    // Getters and Setters
    [[nodiscard]] Vec3f getPosition() const { return position; }
    void setPosition(const Vec3f& pos) { position = pos; }
    [[nodiscard]] Vec2f getSize() const { return size; }
    void setSize(const Vec2f& s) { size = s; }
    [[nodiscard]] Vec3f getNormal() const { return normal; }
    void setNormal(const Vec3f& n) { normal = n; }
    [[nodiscard]] Vec3f getTangent() const { return tangent; }
    void setTangent(const Vec3f& t) { tangent = t; }
private:
    Vec3f position;
    Vec2f size;
    Vec3f normal;
    Vec3f tangent;
};

class Ellipsoid: public Geometry {
public:
    // Construct Ellipsoid as a sphere
    Ellipsoid(const Vec3f& pos): p(pos), a({1, 0, 0}), b({0, 1, 0}), c({0, 0, 1}) {
        aabb = AABB(pos - Vec3f(1, 1, 1), pos + Vec3f(1, 1, 1));
    }
    // Construct Ellipsoid as an ellipsoid
    Ellipsoid(const Vec3f& pos, const Vec3f& a, const Vec3f& b, const Vec3f& c): p(pos), a(a), b(b), c(c) {
        aabb = AABB(pos - a - b - c, pos + a + b + c);
    }

    bool intersect(const Ray& ray, Interaction& interaction) const override;

    std::string getType() const override {
        return "Ellipsoid";
    }
private:
    Vec3f p;
    Vec3f a, b, c;
};

class Ground: public Geometry {
public:
    Ground(float z): z(z) { }

    bool intersect(const Ray& ray, Interaction& interaction) const override;

    std::string getType() const override {
        return "Ground";
    }
protected:
    float z;
};

class Mesh: public Geometry {
public:
    Mesh() = default;
    Mesh(
        const std::vector<Vec3f>& vertices,
        const std::vector<Vec3f>& normals,
        const std::vector<int>& v_indices,
        const std::vector<int>& n_indices
    ): vertices(vertices), normals(normals), 
    v_indices(v_indices), n_indices(n_indices),
    has_accel(0) {
        aabb = AABB(Vec3f(1e8, 1e8, 1e8), Vec3f(-1e8, -1e8, -1e8));
        for (size_t i = 0; i < v_indices.size(); i += 3) {
            Vec3f v0 = vertices[v_indices[i]];
            Vec3f v1 = vertices[v_indices[i+1]];
            Vec3f v2 = vertices[v_indices[i+2]];
            aabb.merge_with(AABB(v0, v1, v2));
        }
    }
    Mesh(const ObjectConfig& object_config);
    
    bool intersect(const Ray& ray, Interaction& interaction) const override;

    void loadObj(const std::string& path);
    void transformObj(Vec3f translation, float scale);

private:
    bool intersectTriangle(const Ray& ray, Interaction& interaction, const Vec3i& v_index, const Vec3i& n_index) const; 
    std::vector<Vec3f> vertices;
    std::vector<Vec3f> normals;
    std::vector<int> v_indices;
    std::vector<int> n_indices;

    int has_accel;
};

#endif // GEOMETRY_HPP_