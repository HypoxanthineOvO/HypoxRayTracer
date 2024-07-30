#include <iostream>

#include "image.hpp"

int main() {
    std::shared_ptr<Image> image = std::make_shared<Image>(800, 800);
    image->writeImage("output.png");
}