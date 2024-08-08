#include <iostream>
#include <chrono>

#include "HypoxRayTracer.hpp"

int main() {
    puts("==========    HypoxRayTracer    ==========");
    
    // Config
    Config config("./configs/small.json");
    
    // Camera
    std::shared_ptr<Image> image = std::make_shared<Image>(config.image_resolution.x(), config.image_resolution.y());
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(config.camera_config, image);

    puts("==========   Camera Generated   ==========");
    // Scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(config);
    puts("==========  Scene  Constructed  ==========");
    // Render
    std::unique_ptr<HypoxRayTracer> RayTracer = std::make_unique<HypoxRayTracer>(camera, scene, config);
    // Time the rendering process
    puts("==========  Rendering  Started  ==========");
    auto start = std::chrono::steady_clock::now();
    RayTracer->render();
    auto end = std::chrono::steady_clock::now();
    printf("Time elapsed: %.2f ms.\n", std::chrono::duration<double, std::milli>(end - start).count());
    puts("==========  Rendering Finished  ==========");
    // Save image
    image->writeImage("output.png");
    puts("==========     Image  Saved     ==========");
}