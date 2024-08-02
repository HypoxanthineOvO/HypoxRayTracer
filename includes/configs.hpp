#ifndef CONFIGS_HPP_
#define CONFIGS_HPP_

#include "utils.hpp"
#include <string>
#include <nlohmann/json.hpp>

enum MaterialType {
    Diffuse,
    Specular
};

struct CameraConfig {
    Vec3f position;
    Vec3f look_at;
    Vec3f ref_up;
    float fov;
    float focal_length;
};

struct LightConfig {
    Vec3f position;
    Vec2f size;
    Vec3f radiance;
};

struct MaterialConfig {
    Vec3f color; // Also named radiance
    MaterialType type;
    std::string name;
};

struct ObjectConfig {
    std::string path;
    std::string material_name;
    Vec3f translate;
    float scale;
};

struct Config {
public:
    Config(std::string config_file_path) {
        loadConfig(config_file_path);
    }
    void loadConfig(std::string config_file_path);

    int spp;
    int max_depth;
    Vec2i image_resolution;
    CameraConfig camera_config;
    std::vector<LightConfig> lights_config;
    std::vector<MaterialConfig> materials_config;
    std::vector<ObjectConfig> objects_config;
};


#endif // CONFIGS_HPP_