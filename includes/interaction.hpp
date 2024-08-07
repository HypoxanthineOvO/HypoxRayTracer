#ifndef INTERACTION_HPP_
#define INTERACTION_HPP_

#include "utils.hpp"


// For HypoxRayTracer
struct MaterialModel {
    Vec3f Ambient;
    Vec3f Diffuse;
    Vec3f Specular;
    float Shininess;
};

struct Interaction {
    enum InterType {
        NONE,
        LIGHT,
        GEOMETRY
    };
    float distance {1e8}; // Distance of the ray
    Vec3f position {0., 0., 0.};
    Vec3f normal {0., 0., 0.};
    InterType type {InterType::NONE};
    Vec2f uv;
    std::shared_ptr<BSDF> material;
    Vec3f w_i {0., 0., 0.};
    Vec3f w_o {0., 0., 0.};

    MaterialModel matmodel;
};


#endif // INTERACTION_HPP_