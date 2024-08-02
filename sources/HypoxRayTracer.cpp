#include "HypoxRayTracer.hpp"
#include <iostream>
#include <iomanip>
// OpenMP
#include <omp.h>
#include "bsdf.hpp"

Vec3f HypoxRayTracer::evalDirectLighting(const Ray& ray, Interaction& interaction, RandomSampler& sampler) const {
    Vec3f color(0, 0, 0);
    if (interaction.material != nullptr) {
        auto vpl = scene->getLight()->getVPL(interaction, sampler);
        Vec3f pos = vpl.position;
        float pdf = vpl.pdf;
        float distance = (pos - interaction.position).norm();
    
        // Shadow Ray
        Ray shadow_ray(interaction.position, (pos - interaction.position).normalized());
        if (scene->isShadowed(shadow_ray) || interaction.material == nullptr) {
            return color;
        }
        else {
            float cos_theta = interaction.normal.dot(shadow_ray.getDirection());
            Vec3f obj_color = interaction.material->evaluate(interaction),
                light_color = scene->getLight()->emmision(pos, -1 * shadow_ray.getDirection());
            color += obj_color.cwiseProduct(light_color) * cos_theta / (
                scene->getLight()->getPDF(interaction) * 2 * distance * distance
            );
        }
    }

    return color;
}

Vec3f HypoxRayTracer::evalRadiance(const Ray& ray, Interaction& interaction, RandomSampler& sampler) const {
    Vec3f color(0, 0, 0);
    Vec3f beta(1, 1, 1);

    Ray ray_for_iteration = std::move(ray);

    for (int i = 0; i < max_depth; i++) {
        Interaction itra;
        if (!scene->intersect(ray_for_iteration, itra) || itra.type == Interaction::InterType::NONE) {
            break;
        }
        itra.w_o = -1 * ray_for_iteration.getDirection();
        if (itra.type == Interaction::InterType::LIGHT) {
            if (i == 0) {
                color += beta.cwiseProduct(scene->getLight()->emmision(itra.position, itra.w_o));
            }
            break;
        }

        // Direct Lighting
        Vec3f direct_lighting = evalDirectLighting(ray_for_iteration, itra, sampler);
        color += beta.cwiseProduct(direct_lighting);

        // Indirect Lighting
        float pdf = itra.material->sample(itra, sampler);
        Ray next_ray(itra.position, itra.w_i);
        ray_for_iteration = next_ray;
        beta = beta.cwiseProduct(
            itra.material->evaluate(itra) * 
            itra.normal.dot(itra.w_i) * PI / pdf
        );
    }


    return color;
}

void HypoxRayTracer::render() {
    Vec2i resolution = camera->getImage()->getResolution();
    int cnt = 0;
    printf("Rendering: %.2f\%", 100.0 * cnt / resolution.x());
    #pragma omp parallel for  schedule(guided, 2), shared(cnt), num_threads(48)
    for(int dx = 0; dx < resolution.x(); dx++) {
        if (dx % (resolution.x() / 200) == 0) {
            //puts("");
            fflush(stdout);
        }
        printf("\rRendering: %.2f%%", 100.0 * cnt / resolution.x());
        
        #pragma omp atomic
        cnt++;

        RandomSampler sampler;
        for(int dy = 0; dy < resolution.y(); dy++) {
            Vec3f color(0, 0, 0);
            sampler.setSeed(omp_get_thread_num());

            // Super Sampling
            auto sample_points = camera->generateSuperSamplingPoint(dx, dy, spp);
            
            for (const auto& sample_point: sample_points) {
                Ray ray = camera->generateRay(sample_point.x(), sample_point.y());
                Interaction interaction;
                if (scene->intersect(ray, interaction)) {
                    color += evalRadiance(ray, interaction, sampler);
                }
            }

            camera->getImage()->setPixel(dx, dy, color / static_cast<float>(spp * spp));
        }
    }
    printf("\n");
}