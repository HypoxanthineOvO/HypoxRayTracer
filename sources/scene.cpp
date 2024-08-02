#include "scene.hpp"

Scene::Scene(const Config& config) {
    // Set light (Now only one light)
    auto light_config = config.lights_config[0];

    light = std::make_shared<SquareAreaLight>(config.lights_config[0]);
    
    ambient_light = Vec3f(0.1, 0.1, 0.1); // TODO: Temporarily set to 0.1

    // Material Config
    std::map<std::string, std::shared_ptr<BSDF>> materials;
    for(const auto& material_config: config.materials_config) {
        std::shared_ptr<BSDF> material = nullptr;
        if (material_config.type == MaterialType::Diffuse) {
            material = std::make_shared<IdealDiffuseBSDF>(material_config);
        }
        else if (material_config.type == MaterialType::Specular) {
            material = std::make_shared<IdealSpecularBSDF>(material_config);
        }
        else {
            puts("Material Type Error!");
        }

        materials[material_config.name] = material;
    }

    for (auto object_config: config.objects_config) {
        auto object = std::make_shared<Mesh>(object_config);
        // Add Materials by name
        object->setMaterial(materials[object_config.material_name]);
        objects.push_back(object);
    }
}

bool Scene::intersect(const Ray& ray, Interaction& interaction) {
    /* Check intersection of ray and this scene */
    Interaction itra;
    itra.distance = ray.getTMax();

    // Check with light
    Interaction itra_light;
    if (
        light->intersect(ray, itra_light) &&
        itra_light.distance > ray.getTMin()
    ) {
        itra = itra_light;
        itra.type = Interaction::InterType::LIGHT;
    }
    // Check with objects
    Interaction itra_obj;
    for (auto object: objects) {
        if (object->intersect(ray, itra_obj)) {
            if (
                itra_obj.distance < itra.distance &&
                itra_obj.distance > ray.getTMin()
            ) {
                itra = itra_obj;
            }
        }
    }

    if (itra.distance > ray.getTMin() ){
        interaction = itra;

        return true;
    }
    return false;
}
