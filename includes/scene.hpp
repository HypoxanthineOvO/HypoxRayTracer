#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "geometry.hpp"
#include "light.hpp"

class Scene{
public:
    Scene() = default;
    virtual ~Scene() = default;

    bool intersect(const Ray& ray, Interaction& interaction);
    bool isShadowed(const Ray& ray) {
        Interaction interaction;
        return (
            intersect(ray, interaction) && 
            interaction.type == Interaction::InterType::GEOMETRY
        );
    };

    void addObject(std::shared_ptr<Geometry> object) {
        objects.push_back(object);
    }
    [[nodiscard]] std::vector<std::shared_ptr<Geometry>> getObjects() const { return objects; }



    // Naive Version
    [[nodiscard]] std::shared_ptr<Light> getLight() const { return light; }
    void setLight(std::shared_ptr<Light> l) { light = l; }
    [[nodiscard]] Vec3f getAmbientLight() const { return ambient_light; }
    void setAmbientLight(const Vec3f& al) { ambient_light = al; }
    // Many Light Method
    //[[nodiscard]] std::vector<std::shared_ptr<Light>> getLights() const { return lights; }
    // void addLight(std::shared_ptr<Light> light) {
    //     lights.push_back(light);
    // }

private:
    std::vector<std::shared_ptr<Geometry>> objects;
    // Naive Version
    std::shared_ptr<Light> light;
    Vec3f ambient_light;
    // Many Light Method
    //std::vector<std::shared_ptr<Light>> lights;
};

#endif // SCENE_HPP_