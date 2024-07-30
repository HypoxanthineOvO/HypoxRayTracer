#include "geometry.hpp"

bool Triangle::intersect(const Ray& ray, Interaction& interaction) const {
    // Moller Trumbore Algorithm
    Interaction result;
    Vec3f o = ray.getOrigin(), d = ray.getDirection();
    float tmin = ray.getTMin(), tmax = ray.getTMax();

    Vec3f e1 = v1 - v0, e2 = v2 - v0;
    Vec3f s = o - v0, s1 = d.cross(e2), s2 = s.cross(e1);
    Vec3f ans = (1 / s1.dot(e1)) * Vec3f(s2.dot(e2), s1.dot(s), s2.dot(d));
    float t = ans.x(), u = ans.y(), v = ans.z();

    if (t >= tmin && u >= 0 && v >= 0 && u + v <= 1) {
        result.distance = t;
        result.position = ray(t);
        result.normal = normal.normalized();
        result.type = Interaction::GEOMETRY;
        result.lightModel = material->evaluate(result);

        interaction = result;
        return true;
    }
    return false;
}


bool Ellipsoid::intersect(const Ray& ray, Interaction& interaction) const {
    // Transform to unit sphere
    Mat4f T, R, S;
    T << 1, 0, 0, p.x(),
         0, 1, 0, p.y(),
         0, 0, 1, p.z(),
         0, 0, 0, 1;
    Vec3f normalized_a = a.normalized(), normalized_b = b.normalized(), normalized_c = c.normalized();
    R << normalized_a.x(), normalized_b.x(), normalized_c.x(), 0,
         normalized_a.y(), normalized_b.y(), normalized_c.y(), 0,
         normalized_a.z(), normalized_b.z(), normalized_c.z(), 0,
         0, 0, 0, 1;
    S << a.norm(), 0, 0, 0,
            0, b.norm(), 0, 0,
            0, 0, c.norm(), 0,
            0, 0, 0, 1;
    Mat4f M = T * R * S;
    Mat4f M_inv = M.inverse();

    Vec4f origin_4d = Vec4f(ray.getOrigin().x(), ray.getOrigin().y(), ray.getOrigin().z(), 1),
            direction_4d = Vec4f(ray.getDirection().x(), ray.getDirection().y(), ray.getDirection().z(), 0);
    
    Vec4f origin_4d_transformed = M_inv * origin_4d, direction_4d_transformed = M_inv * direction_4d;

    Vec3f origin_transformed = Vec3f(origin_4d_transformed.x(), origin_4d_transformed.y(), origin_4d_transformed.z()),
            direction_transformed = Vec3f(direction_4d_transformed.x(), direction_4d_transformed.y(), direction_4d_transformed.z());
    

    // Solve the quadratic equation
    float radius = 1;
    float a = direction_transformed.dot(direction_transformed), 
            b = 2 * origin_transformed.dot(direction_transformed), 
            c = origin_transformed.dot(origin_transformed) - radius * radius;
    float delta = b * b - 4 * a * c;
    
    if (delta > 0) {
        float t1 = (-b - sqrt(delta)) / (2 * a), t2 = (-b + sqrt(delta)) / (2 * a);
        float t;
        if (t1 > 0 && t2 > 0) {
            t = std::min(t1, t2);
        } 
        else if (t1 > 0) {
            t = t1;
        } 
        else if (t2 > 0) {
            t = t2;
        } 
        else {
            return false;
        }

        if (t < ray.getTMin() || t > ray.getTMax()) {
            return false;
        }

        Mat3f M_inv_transpose = M_inv.block<3, 3>(0, 0).transpose();
        Vec3f position = ray(t);
        Vec3f normal = M_inv_transpose * (position);

        Interaction result;
        result.distance = t;
        result.position = position;
        result.normal = normal.normalized();
        result.type = Interaction::GEOMETRY;
        result.lightModel = material->evaluate(result);
        
        interaction = result;
        return true;
    }
    return false;
}

bool Ground::intersect(const Ray& ray, Interaction& interaction) const {
    float t = (z - ray.getOrigin().z()) / ray.getDirection().z();
    if (t < ray.getTMin() || t > ray.getTMax()) {
        return false;
    }

    Interaction result;
    result.distance = t;
    result.position = ray(t);
    result.normal = Vec3f(0, 0, 1);
    result.type = Interaction::GEOMETRY;
    result.lightModel = material->evaluate(result);

    interaction = result;
    return true;
}