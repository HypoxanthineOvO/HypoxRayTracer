#include "material.hpp"

MaterialModel LambertianMaterial::evaluate(Interaction& interaction) const {
    MaterialModel result;
    result.Ambient = ambient_color;
    result.Diffuse = diffuse_color;
    result.Specular = specular_color;
    result.Shininess = shininess;

    return result;
}