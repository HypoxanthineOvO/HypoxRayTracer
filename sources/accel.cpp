#include "accel.hpp"
#include "geometry.hpp"

bool AABB::intersect(const Ray& ray) const{
    float t1 = (xyz_min.x() - ray.getOrigin().x()) / ray.getDirection().x();
    float t2 = (xyz_max.x() - ray.getOrigin().x()) / ray.getDirection().x();
    float tmin = std::min(t1, t2);
    float tmax = std::max(t1, t2);

    for (int i = 1; i < 3; i++) {
        t1 = (xyz_min(i) - ray.getOrigin()(i)) / ray.getDirection()(i);
        t2 = (xyz_max(i) - ray.getOrigin()(i)) / ray.getDirection()(i);
        tmin = std::max(tmin, std::min(t1, t2));
        tmax = std::min(tmax, std::max(t1, t2));
    }

    return tmax > tmin;
}

bool AABB::intersect(const Ray& ray, float* t_in, float* t_out) const{
    float t1 = (xyz_min.x() - ray.getOrigin().x()) / ray.getDirection().x();
    float t2 = (xyz_max.x() - ray.getOrigin().x()) / ray.getDirection().x();
    float tmin = std::min(t1, t2);
    float tmax = std::max(t1, t2);

    for (int i = 1; i < 3; i++) {
        t1 = (xyz_min(i) - ray.getOrigin()(i)) / ray.getDirection()(i);
        t2 = (xyz_max(i) - ray.getOrigin()(i)) / ray.getDirection()(i);
        tmin = std::max(tmin, std::min(t1, t2));
        tmax = std::min(tmax, std::max(t1, t2));
    }

    *t_in = tmin;
    *t_out = tmax;

    return tmax > tmin;
}
