#include <iostream>
#include <chrono>

#include "HypoxRayTracer.hpp"

// Initialize Scene
void Generate_Camera_Scene(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene) {
    /* Generate a Conell Box and related camera */

}

int main() {
    puts("==========    HypoxRayTracer    ==========");
    
    // Camera
    std::shared_ptr<Image> image = std::make_shared<Image>(400, 400);
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->setImage(image);

    puts("==========   Camera Generated   ==========");
    // Scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    Generate_Camera_Scene(camera, scene);
    puts("==========  Scene  Constructed  ==========");
    // Render
    std::unique_ptr<HypoxRayTracer> RayTracer = std::make_unique<HypoxRayTracer>(camera, scene);
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