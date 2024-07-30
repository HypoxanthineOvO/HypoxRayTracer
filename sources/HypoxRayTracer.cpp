#include "HypoxRayTracer.hpp"

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
        float diff_factor = 0, spec_factor = 0;

        Vec3f light_dir = (vpl.position - interaction.position).normalized();
        Ray shadow_ray(
            interaction.position, 
            light_dir
        );
        if (scene->isShadowed(shadow_ray)) {
            continue;
        }
        else {
            diff_factor = std::max(0.0f, interaction.normal.dot(light_dir));
            
            Vec3f half = (light_dir - ray.getDirection()).normalized();
            spec_factor = std::max(
                0.0f, 
                powf(std::max(0.0f, interaction.normal.dot(half)), interaction.matmodel.Shininess)
            );
        
            diff += (
                interaction.matmodel.Diffuse.cwiseProduct(vpl.color) * diff_factor
            );
            spec += (
                interaction.matmodel.Specular.cwiseProduct(vpl.color) * spec_factor
            );
        }
    }
    return amb + diff + spec;
}

void HypoxRayTracer::render() {
    Vec2i resolution = camera->getImage()->getResolution();
    int cnt = 0;

    for(int dx = 0; dx < resolution.x(); dx++) {
        printf("\r%.02f%%", cnt * 100.0 / resolution.x());
        cnt++;
        for(int dy = 0; dy < resolution.y(); dy++) {
            Vec3f color(0, 0, 0);

            Ray ray = camera->generateRay(dx, dy);
            Interaction interaction;
            if (scene->intersect(ray, interaction)) {
                puts("Intersected!");
                color += evalRadiance(ray, interaction);
                //printf("Pixel[%d, %d] Color: %f %f %f\n", dx, dy, color.x(), color.y(), color.z());
                color += Vec3f(1, 1, 1);
            }
            camera->getImage()->setPixel(dx, dy, color);
        }
    }
    printf("\r%.02f%%\n", cnt * 100.0 / resolution.x());
}