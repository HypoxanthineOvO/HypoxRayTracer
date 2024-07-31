#include "scene.hpp"

bool Scene::intersect(const Ray& ray, Interaction& interaction) {
    /* Check intersection of ray and this scene */
    Interaction itra;
    itra.distance = ray.getTMax();

    // Check with light
    Interaction itra_light;
    if (
        light->intersect(ray, itra_light) &&
        itra_light.distance > ray.getTMin()// &&
        //itra_light.distance < ray.getTMax()
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
                itra_obj.distance > ray.getTMin() //&&
                //itra_obj.distance < ray.getTMax()
            ) {
                itra = itra_obj;
            }
        }
    }

    if (itra.distance > ray.getTMin() ){//& itra.distance < ray.getTMax()) {
        interaction.position = itra.position;
        interaction.normal = itra.normal;
        interaction.distance = itra.distance;
        interaction.matmodel = itra.matmodel;
        interaction.type = itra.type;
        
        return true;
    }
    return false;
}