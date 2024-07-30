#ifndef INTERACTION_HPP_
#define INTERACTION_HPP_

#include "utils.hpp"

// For HypoxRayTracer
struct LightModel {
    Vec3f Light_Ambient;
    Vec3f Light_Diffuse;
    Vec3f Light_Specular;
    float Light_Shininess;
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
    LightModel lightModel;
};

// For Light Former Front End
class GBufferEntry {

};


#endif // INTERACTION_HPP_