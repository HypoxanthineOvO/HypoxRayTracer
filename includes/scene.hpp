#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "geometry.hpp"

class Scene{
public:

private:
    std::vector<std::shared_ptr<Geometry>> objects;
};

#endif // SCENE_HPP_