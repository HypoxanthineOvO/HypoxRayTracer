#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "interaction.hpp"

class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    [[nodisgard]] virtual MaterialModel evaluate(
        Interaction& interaction
    ) const = 0;
};

class LambertianMaterial: public Material {
public:
    LambertianMaterial():
        ambient_color({1, 1, 1}), diffuse_color({1, 1, 1}), specular_color({1, 1, 1}), shininess(16.0f) {}
    LambertianMaterial(const Vec3f& color, float shininess = 16.0f):
        ambient_color(color), diffuse_color(color), specular_color({1, 1, 1}), shininess(shininess) {}
    LambertianMaterial(const Vec3f& color, const Vec3f& specular_color, float shininess = 16.0f):
        ambient_color(color), diffuse_color(color), specular_color(specular_color), shininess(shininess) {}

    [[nodiscard]] MaterialModel evaluate(
        Interaction& interaction
    ) const override;

private:
    Vec3f ambient_color;
    Vec3f diffuse_color;
    Vec3f specular_color;
    float shininess;
};

#endif // MATERIAL_HPP_