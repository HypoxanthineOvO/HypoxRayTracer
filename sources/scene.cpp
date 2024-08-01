#include "scene.hpp"

Scene::Scene(const Config& config) {
    // Set light (Now only one light)
    auto light_config = config.lights_config[0];

    light = std::make_shared<SquareAreaLight>(config.lights_config[0]);
    
    ambient_light = Vec3f(0.1, 0.1, 0.1); // TODO: Temporarily set to 0.1

    // Set objects
    std::shared_ptr<Material> red_mat = std::make_shared<LambertianMaterial>(Vec3f(0.63, 0.065, 0.05));
    std::shared_ptr<Material> green_mat = std::make_shared<LambertianMaterial>(Vec3f(0.14, 0.45, 0.091));
    std::shared_ptr<Material> grey_mat = std::make_shared<LambertianMaterial>(Vec3f(0.725, 0.71, 0.68));
    
    for (auto object_config: config.objects_config) {
        auto object = std::make_shared<Mesh>(object_config);
        // TODO: Add true material
        if (object_config.material_name == "red_diffuse") {
            object->setMaterial(red_mat);
            puts("RED");
        }
        else if (object_config.material_name == "green_diffuse"){
            object->setMaterial(green_mat);
            puts("GREEN");
        }
        else {
            object->setMaterial(grey_mat);
            puts("GREY");
        }
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
        interaction.position = itra.position;
        interaction.normal = itra.normal;
        interaction.distance = itra.distance;
        interaction.matmodel = itra.matmodel;
        interaction.type = itra.type;
        
        return true;
    }
    return false;
}
