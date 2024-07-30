#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "utils.hpp"
#include <vector>

class Image {
public:
    Image(): resolution(800, 800){
        data.resize(resolution.x() * resolution.y());
    }
    Image(int w_h): resolution(w_h, w_h) {
        data.resize(resolution.x() * resolution.y());
    }
    Image(int weight, int height): resolution(weight, height) {
        data.resize(resolution.x() * resolution.y());
    }

    [[nodiscard]] float getAspectRatio() const {
        return static_cast<float>(resolution.x()) / static_cast<float>(resolution.y());
    }
    [[nodiscard]] Vec2i getResolution() const {
        return resolution;
    }
    
    Vec3f getPixel(int x, int y) const {
        return data[x + y * resolution.x()];
    }
    void setPixel(int x, int y, const Vec3f& color) {
        data[x + y * resolution.x()] = color;
    }

    void showImage();
    void writeImage(const std::string& filename);
private:
    std::vector<Vec3f> data;
    Vec2i resolution;
};


#endif // IMAGE_HPP_