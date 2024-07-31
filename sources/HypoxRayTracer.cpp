#include "HypoxRayTracer.hpp"
#include <iostream>
#include <iomanip>
// OpenMP
#include <omp.h>

Vec3f HypoxRayTracer::evalRadiance(const Ray& ray, Interaction& interaction) const {
    // Direct Intersect with Light
    if (interaction.type == Interaction::InterType::LIGHT) {
        return scene->getLight()->getColor();
    }
    
    Vec3f amb(0, 0, 0), diff(0, 0, 0), spec(0, 0, 0);

    // Ambient Part
    amb = interaction.matmodel.Ambient.cwiseProduct(scene->getAmbientLight());

    // Diffuse Part and Specular Part
    auto vpls = scene->getLight()->getVPLs();
    for (auto vpl: vpls) {
        Vec3f light_dir = (vpl.position - interaction.position).normalized();
        Ray shadow_ray(
            interaction.position, 
            light_dir + 0.01 * interaction.normal
        );
        if (scene->isShadowed(shadow_ray)) {
            continue;
        }
        else {
            float diff_factor = std::max(0.0f, interaction.normal.dot(light_dir));
            
            Vec3f reflect_dir = (2 * interaction.normal.dot(light_dir) * interaction.normal - light_dir).normalized();
            float spec_factor = powf(
                std::max(0.0f, reflect_dir.dot(-ray.getDirection())), 
                interaction.matmodel.Shininess
            );
            
            diff += (
                interaction.matmodel.Diffuse.cwiseProduct(vpl.color) * diff_factor / vpls.size()
            );
            spec += (
                interaction.matmodel.Specular.cwiseProduct(vpl.color) * spec_factor / vpls.size()
            );

        }
    }
    return amb + diff + spec;
}

void HypoxRayTracer::render() {
    Vec2i resolution = camera->getImage()->getResolution();
    int cnt = 0;

    #pragma omp parallel for  schedule(guided, 2), shared(cnt), num_threads(24)
    for(int dx = 0; dx < resolution.x(); dx++) {
        if (dx % 10 == 0) {
        }
        #pragma omp atomic
        cnt++;
        
        for(int dy = 0; dy < resolution.y(); dy++) {
            Vec3f color(0, 0, 0);

            Ray ray = camera->generateRay(dx, dy);
            
            Interaction interaction;
            if (scene->intersect(ray, interaction)) {
                color += evalRadiance(ray, interaction);
            }
            camera->getImage()->setPixel(dx, dy, color);
        }
    }
}