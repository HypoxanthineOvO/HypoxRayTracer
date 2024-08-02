#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include "utils.hpp"
#include "camera.hpp"
#include "interaction.hpp"

struct VPL {
    VPL(const Vec3f& position, const Vec3f& color, float pdf): position(position), color(color), pdf(pdf) {}
    VPL(const Vec3f& position, const Vec3f& color): position(position), color(color), pdf(1.0f) {}
    VPL(const Vec3f& position): position(position), color(Vec3f(1, 1, 1)), pdf(1.0f) {}
    VPL(const Vec3f& position, float pdf): position(position), color(Vec3f(1, 1, 1)), pdf(pdf) {}
    Vec3f position;
    Vec3f color;
    float pdf;
};

typedef std::vector<VPL> VPLs;

class Light {
public:
    Light(const Vec3f& position, const Vec3f& color): position(position), radiance(color) {}
    Light(const LightConfig& light_config): position(light_config.position), radiance(Vec3f(1, 1, 1)) {}
    virtual ~Light() = default;

    [[nodisgard]] virtual Vec3f emmision(const Vec3f& pos, const Vec3f& dir) const = 0;

    [[nodiscard]] virtual VPL getVPL(Interaction& interaction, RandomSampler& sampler) const = 0;
    virtual float getPDF(const Interaction& interaction) const = 0;


    virtual bool intersect(const Ray& ray, Interaction& interaction) const = 0;

    // Getters and Setters
    [[nodiscard]] Vec3f getPosition() const { return position; }
    void setPosition(const Vec3f& pos) { position = pos; }
    [[nodiscard]] Vec3f getColor() const { return radiance; }
    void setColor(const Vec3f& c) { radiance = c; }
protected:
    Vec3f position;
    Vec3f radiance;
};

class SquareAreaLight: public Light {
public:
    SquareAreaLight(
        const Vec3f& position,
        const Vec3f& color,
        const Vec2f& size,
        const Vec3f& normal,
        const Vec3f& tangent
    ): Light(position, color), normal(normal), size(size), tangent(tangent) {}

    SquareAreaLight(const LightConfig& light_config):
        Light(light_config.position, light_config.radiance) {
        size = light_config.size;
        // TODO (Be Modified in future)
        normal = Vec3f(0, -1, 0);
        tangent = Vec3f(1, 0, 0);
    }

    [[nodisgard]] virtual Vec3f emmision(const Vec3f& pos, const Vec3f& dir) const override;
    [[nodiscard]] virtual VPL getVPL(Interaction& interaction,  RandomSampler& sampler) const override;
    virtual float getPDF(const Interaction& interaction) const override;
    
    bool intersect(const Ray& ray, Interaction& interaction) const override;

protected:
    Vec3f normal;
    Vec2f size;
    Vec3f tangent;

};

#endif // LIGHT_HPP_