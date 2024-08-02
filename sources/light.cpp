#include "light.hpp"
#include "geometry.hpp"

Vec3f SquareAreaLight::emmision(const Vec3f& pos, const Vec3f& dir) const {
    // For Square Light: Only consider the angle between the normal and the direction
    float cos_theta = std::max(
        0.0f, normal.normalized().dot(dir.normalized())
    );
    return cos_theta * radiance;
}

VPL SquareAreaLight::getVPL(Interaction& interaction,  RandomSampler& sampler) const {
    // Do random sampling.
    // For Square Light: Uniform Distribution
    float dx = sampler.get1D(), dy = sampler.get1D();
    
    float pdf = 1.0f / (size.x() * size.y());

    // Consider the position and the normal
    Vec3f tangent_y = normal.cross(tangent);
    Vec3f sample_pos = position + (dx - 0.5) * size.x() * tangent + (dy - 0.5) * size.y() * tangent_y;
    return VPL(sample_pos, radiance, pdf);
}

float SquareAreaLight::getPDF(const Interaction& interaction) const {
    // For Square Light: Uniform Distribution
    float pdf = 1.0f / (size.x() * size.y());
    return pdf;
}

bool SquareAreaLight::intersect(const Ray& ray, Interaction& interaction) const {

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