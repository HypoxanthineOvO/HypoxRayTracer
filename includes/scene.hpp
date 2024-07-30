#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "geometry.hpp"
#include "light.hpp"

class Scene{
public:

private:
    std::vector<std::shared_ptr<Geometry>> objects;
    std::vector<std::shared_ptr<Light>> lights;
};

#endif // SCENE_HPP_