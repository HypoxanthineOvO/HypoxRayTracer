#ifndef HYPOX_RAY_TRACER_HPP_
#define HYPOX_RAY_TRACER_HPP_

#include "camera.hpp"
#include "scene.hpp"

class HypoxRayTracer {
public:
    HypoxRayTracer() = delete;
    HypoxRayTracer(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene, int spp = 1, int max_depth = 3): 
        camera(camera), scene(scene), spp(spp), max_depth(max_depth) {}

    void render();

private:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;
    int spp, max_depth;
};

#endif // HYPOX_RAY_TRACER_HPP_