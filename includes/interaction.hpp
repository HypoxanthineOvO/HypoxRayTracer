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
    float distance; // Distance of the ray
    Vec3f position;
    Vec3f normal;
    InterType type;
    Vec2f uv;
    MaterialModel matmodel;
};

// For Light Former Front End
class GBufferEntry {

};


#endif // INTERACTION_HPP_