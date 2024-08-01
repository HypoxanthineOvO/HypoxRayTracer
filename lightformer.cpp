#ifndef LIGHT_FORMER_FRONT_END_HPP_
#define LIGHT_FORMER_FRONT_END_HPP_

#include "camera.hpp"
#include "scene.hpp"

struct DirectVPLsData {
    Vec3f position;
    Vec3f normal;
    Vec3f power;
};

struct IndirectVPLsData {
    Vec3f position;
    Vec3f normal;
    Vec3f flux;
};

struct ShadowData {
    float d_sub_df;
    float d_div_df;
    float c_e;
    float c_c;
    Vec3f position;
};

struct GBufferEntry {
    Vec3f position;
    Vec3f normal;
    Vec3f albedo;
    Vec3f specular;
    float roughness;
};

class LightFormerFrontEnd {
public:
    using LightDirection = Vec3f;
    using HalfVector = Vec3f;
private:
    // Basic Components: Camera and Scene
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;

    // Buffers For Light Former
    std::vector<DirectVPLsData> direct_vpls;
    std::vector<IndirectVPLsData> indirect_vpls;
    std::vector<LightDirection> light_directions;
    std::vector<HalfVector> half_vectors;
    std::vector<ShadowData> shadow_data;
    std::vector<GBufferEntry> gbuffer;
};

#endif // LIGHT_FORMER_FRONT_END_HPP_