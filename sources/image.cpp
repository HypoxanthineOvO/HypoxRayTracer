#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "image.hpp"

void Image::writeImage(const std::string& filename) {
    std::vector<uint8_t> rgb_data(resolution.x() * resolution.y() * 3);
    for (int i = 0; i < resolution.x() * resolution.y(); i++) {
        Vec3f color = data[i];
        rgb_data[i * 3] = utils::gammaCorrection(color.x());
        rgb_data[i * 3 + 1] = utils::gammaCorrection(color.y());
        rgb_data[i * 3 + 2] = utils::gammaCorrection(color.z());
    }

    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename.c_str(), resolution.x(), resolution.y(), 3, rgb_data.data(), resolution.x() * 3);
}