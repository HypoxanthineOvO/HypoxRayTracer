#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "utils.hpp"
#include "image.hpp"

class Camera {
public:

private:
    Vec3f position;
    Vec3f forward, right, up;
    float fov;
    float focal_length;

    std::shared_ptr<Image> image;
};


#endif // CAMERA_HPP_