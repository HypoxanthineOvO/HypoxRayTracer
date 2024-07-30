#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "utils.hpp"
#include "image.hpp"


class Ray {
public:
    Ray(const Vec3f& origin, const Vec3f& direction): origin(origin), direction(direction), t_min(0.1), t_max(100) {}
    
    [[nodiscard]] Vec3f at (float t) const {
        return origin + t * direction;
    }
    [[nodiscard]] Vec3f operator() (float t) const {
        return at(t);
    }

    // Getter
    [[nodiscard]] Vec3f getOrigin() const { return origin; }
    [[nodiscard]] Vec3f getDirection() const { return direction; }
    [[nodiscard]] float getTMin() const { return t_min; }
    [[nodiscard]] float getTMax() const { return t_max; }
    // Setter
    void setOrigin(const Vec3f& origin) { this->origin = origin; }
    void setDirection(const Vec3f& direction) { this->direction = direction; }
    void setTMin(float t_min) { this->t_min = t_min; }
    void setTMax(float t_max) { this->t_max = t_max; }

private:
    Vec3f origin, direction;
    float t_min, t_max;
};

class Camera {
public:
    Camera(): fov(45), focal_length(1.0), image(nullptr) {}
    Camera(std::shared_ptr<Image> image): position({0, -1, 0}), fov(45), focal_length(1.0), image(image) {
        lookAt(Vec3f(0, 0, 0), Vec3f(0, 0, 1));
    }
    

    void lookAt(const Vec3f look_at, const Vec3f& ref_up = Vec3f(0, 1, 0)) {
        forward = -(look_at - position).normalized();
        right = ref_up.cross(forward).normalized();
        up = forward.cross(right).normalized();
        // Generate x y scale
        float y_scale = tanf(utils::radians(fov / 2)), x_scale = y_scale * image->getAspectRatio();
        up *= y_scale * focal_length;
        right *= x_scale * focal_length;
    }

    Ray generateRay(float dx, float dy) {
        // Get the position of screen center.
        Vec3f screen_center = position - focal_length * forward;

        // Get the bias of screen point
        Vec2i resolution = getImage()->getResolution();
        float aspect_ratio = getImage()->getAspectRatio();
        Vec3f delta_x = (2 * ((dx ) / (resolution.x())) - 1)
            * focal_length * aspect_ratio * tanf(fov * PI / 360)
            * right;
        Vec3f delta_y = (2 * ((dy ) / (resolution.y())) - 1)
            * focal_length * tanf(fov * PI / 360)
            * up;
        Vec3f screen_point_location = screen_center + delta_x + delta_y;
        return Ray(position, (screen_point_location-position));
    }

    // Getter
    [[nodiscard]] Vec3f getPosition() const { return position; }
    [[nodiscard]] Vec3f getForward() const { return forward; }
    [[nodiscard]] Vec3f getRight() const { return right; }
    [[nodiscard]] Vec3f getUp() const { return up; }
    [[nodiscard]] float getFov() const { return fov; }
    [[nodiscard]] float getFocalLength() const { return focal_length; }
    [[nodiscard]] std::shared_ptr<Image> getImage() const { return image; }
    // Setter
    void setPosition(const Vec3f& position) { this->position = position; }
    void setForward(const Vec3f& forward) { this->forward = forward; }
    void setRight(const Vec3f& right) { this->right = right; }
    void setUp(const Vec3f& up) { this->up = up; }
    void setFov(float fov) { this->fov = fov; }
    void setFocalLength(float focal_length) { this->focal_length = focal_length; }
    void setImage(const std::shared_ptr<Image>& image) { this->image = image; }
private:
    Vec3f position;
    Vec3f forward, right, up;
    float fov;
    float focal_length;

    std::shared_ptr<Image> image;
};


#endif // CAMERA_HPP_