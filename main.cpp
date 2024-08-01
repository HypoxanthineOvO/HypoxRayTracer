#include <iostream>
#include <chrono>

#include "HypoxRayTracer.hpp"

// Initialize Scene
void Generate_Camera_Scene(std::shared_ptr<Camera> camera, std::shared_ptr<Scene> scene) {
    /* Generate a Conell Box and related camera */
    // Camera part
    Vec3f camera_pos = Vec3f(0, -3, 1.3), camera_lookat = Vec3f(0, 0, 1);
    camera->setPosition(camera_pos);
    camera->lookAt(camera_lookat);
    camera->setFov(45);

    // Light part
    Vec3f light_position = Vec3f(0, 0, 1.95),
            light_color = Vec3f(1, 1, 1),
            light_normal = Vec3f(0, 0, -1);
    Vec2f light_size = Vec2f(0.5, 0.3);
    std::shared_ptr<Light> square_light = std::make_shared<SquareAreaLight>(light_position, light_color, light_normal, light_size, Vec3f(1, 0, 0));
    scene->setLight(square_light);
    scene->setAmbientLight(Vec3f(0.1, 0.1, 0.1));


    // Object Part
    // 1. Materials
    std::shared_ptr<Material> white_mat = std::make_shared<LambertianMaterial>(Vec3f(0.7, 0.7, 0.7));
    std::shared_ptr<Material> red_mat = std::make_shared<LambertianMaterial>(Vec3f(0.8, 0, 0));
    std::shared_ptr<Material> green_mat = std::make_shared<LambertianMaterial>(Vec3f(0, 0.8, 0));
    std::shared_ptr<Material> blue_mat = std::make_shared<LambertianMaterial>(Vec3f(0.2, 0.5, 0.9));
    std::shared_ptr<Material> yellow_mat = std::make_shared<LambertianMaterial>(Vec3f(1, 0.93, 0.6));
    
    // 2. Geometries
	// 2.1. Box
    std::shared_ptr<Geometry> floor = std::make_shared<Rectangle>(Vec3f(0, 0, 0), Vec2f(2, 2), Vec3f(0, 0, 1), Vec3f(1, 0, 0));
    floor->setMaterial(white_mat);
	std::shared_ptr<Geometry> ceiling = std::make_shared<Rectangle>(Vec3f(0, 0, 2), Vec2f(2, 2), Vec3f(0, 0, -1), Vec3f(1, 0, 0));
    ceiling->setMaterial(white_mat);
	std::shared_ptr<Geometry> wall_left = std::make_shared<Rectangle>(Vec3f(-1, 0, 1), Vec2f(2, 2), Vec3f(1, 0, 0), Vec3f(0, 0, 1));
    wall_left->setMaterial(red_mat);
	std::shared_ptr<Geometry> wall_right = std::make_shared<Rectangle>(Vec3f(1, 0, 1), Vec2f(2, 2), Vec3f(-1, 0, 0), Vec3f(0, 0, -1));
    wall_right->setMaterial(green_mat);
	std::shared_ptr<Geometry> wall_back = std::make_shared<Rectangle>(Vec3f(0, 1, 1), Vec2f(2, 2), Vec3f(0, -1, 0), Vec3f(1, 0, 0));
    wall_back->setMaterial(white_mat);
    // 2.2. Tetrahedron
    Vec3f vert_0 = Vec3f(-1, 0.2, 0.2),
            vert_1 = Vec3f(-0.5, -0.65, 0.2),
            vert_2 = Vec3f(0, 0.2, 0.2),
            vert_3 = Vec3f(-0.5, -0.3774, 1);
    std::shared_ptr<Geometry> tetrahedron_0 = std::make_shared<Triangle>(vert_0, vert_1, vert_2),
            tetrahedron_1 = std::make_shared<Triangle>(vert_0, vert_1, vert_3),
            tetrahedron_2 = std::make_shared<Triangle>(vert_0, vert_3, vert_2),
            tetrahedron_3 = std::make_shared<Triangle>(vert_1, vert_2, vert_3);
    tetrahedron_0->setMaterial(blue_mat);
    tetrahedron_1->setMaterial(blue_mat);
    tetrahedron_2->setMaterial(blue_mat);
    tetrahedron_3->setMaterial(blue_mat);
    // 2.3. Ellipsoid
    std::shared_ptr<Geometry> ellipsoid = std::make_shared<Ellipsoid>(Vec3f(0.5, 0.4, 0.4), Vec3f(0.3, 0.1, 0), Vec3f(-0.1, 0.3, 0), Vec3f(0, 0, 0.4));
    ellipsoid->setMaterial(yellow_mat);

    scene->addObject(floor);
    scene->addObject(ceiling);
    scene->addObject(wall_left);
    scene->addObject(wall_right);
    scene->addObject(wall_back);
    scene->addObject(tetrahedron_0), scene->addObject(tetrahedron_1), scene->addObject(tetrahedron_2), scene->addObject(tetrahedron_3);
    scene->addObject(ellipsoid);
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
    std::unique_ptr<HypoxRayTracer> RayTracer = std::make_unique<HypoxRayTracer>(camera, scene, 4);
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