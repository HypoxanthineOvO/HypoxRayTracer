#include <iostream>

#include "HypoxRayTracer.hpp"

int main() {
    puts("==========    HypoxRayTracer    ==========");
    
    // Camera
    std::shared_ptr<Image> image = std::make_shared<Image>(800, 800);
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->setImage(image);
    puts("==========   Camera Generated   ==========");
    // Scene
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    puts("==========  Scene  Constructed  ==========");
    // Render
    std::unique_ptr<HypoxRayTracer> rayTracer = std::make_unique<HypoxRayTracer>(camera, scene);
    puts("==========  Rendering  Started  ==========");
    rayTracer->render();
    puts("==========  Rendering Finished  ==========");
    // Save image
    image->writeImage("output.png");
    puts("==========     Image  Saved     ==========");
}