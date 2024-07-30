#include "light.hpp"
#include "geometry.hpp"

VPLs SquareAreaLight::getVPLs() const {
    VPLs vpls;

    int numVPLs_per_side = static_cast<int>(std::sqrt(numDirectVPLs));
    
    Vec3f u = tangent.normalized() * size.x(), 
            v = normal.cross(tangent).normalized() * size.y();
    
    for (int i = 0; i < numVPLs_per_side; i++) {
        for (int j = 0; j < numVPLs_per_side; j++) {
            int offset = numVPLs_per_side / 2;
            Vec3f pos = position +
                u * (i - offset) / numVPLs_per_side +
                v * (j - offset) / numVPLs_per_side;
            vpls.push_back(VPL(pos, color));
        }
    }

    return vpls;
}

bool SquareAreaLight::intersect(const Ray& ray, Interaction& interaction) const {
    Vec3f origin = ray.getOrigin(), direction = ray.getDirection();

    if (direction.dot(normal) <= EPS) return false;

    // Construct the Rectangle
    Rectangle light_rect = Rectangle(position, size, normal, tangent);

    Interaction result;
    if (light_rect.intersect(ray, result)) {
        interaction = result;

        interaction.type = Interaction::LIGHT; // Only need to set the type
        
        return true;
    }
    return false;
}