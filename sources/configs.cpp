#include "configs.hpp"

#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"

void Config::loadConfig(std::string config_file_path) {
    /* Load Config From File */
    printf("Loading config file: %s\n", config_file_path.c_str());

    // Load JSON
    std::ifstream config_file;
    config_file.open(config_file_path);
    if (!config_file.is_open()) {
        printf("Failed to open config file: %s\n", config_file_path.c_str());
        exit(1);
    }
    nlohmann::json raw;
    config_file >> raw;
    config_file.close();

    printf("Parsing config file: - ");
    // Parse the json
    // Basic Configs
    raw["spp"].get_to(spp);
    raw["max_depth"].get_to(max_depth);
    int img_w, img_h;
    raw["image_resolution"][0].get_to(img_w);
    raw["image_resolution"][1].get_to(img_h);
    image_resolution = Vec2i(img_w, img_h);
    printf("Basic Config - ");
    // Camera Configs
    auto camera = raw["cam_config"];
    camera["position"][0].get_to(camera_config.position.x());
    camera["position"][1].get_to(camera_config.position.y());
    camera["position"][2].get_to(camera_config.position.z());
    camera["look_at"][0].get_to(camera_config.look_at.x());
    camera["look_at"][1].get_to(camera_config.look_at.y());
    camera["look_at"][2].get_to(camera_config.look_at.z());
    camera["ref_up"][0].get_to(camera_config.ref_up.x());
    camera["ref_up"][1].get_to(camera_config.ref_up.y());
    camera["ref_up"][2].get_to(camera_config.ref_up.z());
    camera["vertical_fov"].get_to(camera_config.fov);

    camera["focal_length"].get_to(camera_config.focal_length);

    printf("Camera Config - ");
    // Light Configs
    for (auto light : raw["light_config"]) {
        LightConfig light_config;
        light["position"][0].get_to(light_config.position.x());
        light["position"][1].get_to(light_config.position.y());
        light["position"][2].get_to(light_config.position.z());
        light["size"][0].get_to(light_config.size.x());
        light["size"][1].get_to(light_config.size.y());
        light["radiance"][0].get_to(light_config.radiance.x());
        light["radiance"][1].get_to(light_config.radiance.y());
        light["radiance"][2].get_to(light_config.radiance.z());
        lights_config.push_back(light_config);
    }
    printf("Light Config - ");
    // Material Configs
    for (auto material : raw["materials"]) {
        MaterialConfig material_config;
        material["color"][0].get_to(material_config.color.x());
        material["color"][1].get_to(material_config.color.y());
        material["color"][2].get_to(material_config.color.z());
        std::string type = material["type"];
        if (type == "diffuse") {
            material_config.type = MaterialType::Diffuse;
        } else if (type == "specular") {
            material_config.type = MaterialType::Specular;
        }
        material["name"].get_to(material_config.name);
        materials_config.push_back(material_config);
    }
    printf("Material Config - ");
    // Object Configs
    for (auto object : raw["objects"]) {
        ObjectConfig object_config;
        object_config.path = object["obj_file_path"];
        object_config.material_name = object["material_name"];
        object["translate"][0].get_to(object_config.translate.x());
        object["translate"][1].get_to(object_config.translate.y());
        object["translate"][2].get_to(object_config.translate.z());
        object["scale"].get_to(object_config.scale);
        objects_config.push_back(object_config);
    }
    printf("Object Config -\n");
}