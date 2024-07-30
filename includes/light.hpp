#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "utils.hpp"
#include "camera.hpp"
#include "interaction.hpp"

struct VPL {
    VPL(const Vec3f& position, const Vec3f& color): position(position), color(color) {}
    Vec3f position;
    Vec3f color;
};

typedef std::vector<VPL> VPLs;

class Light {
public:
    Light(const Vec3f& position, const Vec3f& color): position(position), color(color) {}
    virtual ~Light() = default;

    [[nodiscard]] virtual VPLs getVPLs() const = 0;
    virtual bool intersect(const Ray& ray, Interaction& interaction) const = 0;

    // Getters and Setters
    [[nodiscard]] Vec3f getPosition() const { return position; }
    void setPosition(const Vec3f& pos) { position = pos; }
    [[nodiscard]] Vec3f getColor() const { return color; }
    void setColor(const Vec3f& c) { color = c; }
protected:
    Vec3f position;
    Vec3f color;
};


class EnvironmentLight: public Light {
public:
    EnvironmentLight(const Vec3f& color): Light(Vec3f(0, 0, 0), color) {}

    VPLs getVPLs() const override {
        return {};
    }
    bool intersect(const Ray& ray, Interaction& interaction) const override {
        return false;
    }
};

class SquareAreaLight: public Light {
public:
    SquareAreaLight(
        const Vec3f& position,
        const Vec3f& color,
        const Vec3f& normal,
        const Vec2f& size,
        const Vec3f& tangent
    ): Light(position, color), normal(normal), size(size), tangent(tangent) {}

    [[nodiscard]] VPLs getVPLs() const override;
    bool intersect(const Ray& ray, Interaction& interaction) const override;

protected:
    Vec3f normal;
    Vec2f size;
    Vec3f tangent;

    int numDirectVPLs = 25;
};

#endif // LIGHT_HPP_