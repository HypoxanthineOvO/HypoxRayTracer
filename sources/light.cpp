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
    //Vec3f origin = ray.getOrigin(), direction = ray.getDirection();
    Vec3f o = ray.getOrigin(), d = ray.getDirection();
    float width = size.x(), height = size.y();

    if (std::abs(d.dot(normal)) <= EPS) {
        return false;
    }
    // Intersect with the plane
    float t = (position - o).dot(normal) / d.dot(normal);
    Vec3f intersect_point = ray(t);
    Vec3f delta_vec = intersect_point - position;

    Vec3f y_tangent = normal.cross(tangent);
    float dw = delta_vec.dot(tangent.normalized()), dh = delta_vec.dot(y_tangent.normalized());

    if (t >= 0 && -width/2 <= dw && dw <= width/2 && -height/2 <= dh && dh <= height/2) {
        interaction.distance = t;
        interaction.position = intersect_point;
        interaction.normal = normal.normalized();
        interaction.type = Interaction::InterType::LIGHT;


        return true;
    }
    return false;
}