#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <memory>


#ifdef DEBUG
#include <iostream>
#endif

// Eigen Part
#include <Eigen/Core>
#include <Eigen/Dense>

template <typename T, int size>
using Vec = Eigen::Matrix<T, size, 1>;

using Vec3f = Eigen::Vector3f;
using Vec3i = Eigen::Vector3i;
using Vec2f = Eigen::Vector2f;
using Vec2i = Eigen::Vector2i;
using Vec4f = Eigen::Vector4f;
using Mat3f = Eigen::Matrix3f;
using Mat4f = Eigen::Matrix4f;


constexpr float RAY_DEFAULT_MIN = 1e-5;
constexpr float RAY_DEFAULT_MAX = 1e7;
constexpr float PI = 3.141592653579f;
constexpr float INV_PI = 0.31830988618379067154;
constexpr float EPS = 1e-5;
constexpr float ROTATE_ANGLE = 22.6f;

// Utils Part
namespace utils {
	
	static inline float clamp(float v, float min, float max) {
		if (v > max) v = max;
		else if (v < min) v = min;
		return v;
	}
	static inline Vec3f clamp(const Vec3f& v, Vec3f min, Vec3f max) {
		return Vec3f(clamp(v.x(), min.x(), max.x()), clamp(v.y(), min.y(), max.y()), clamp(v.z(), min.z(), max.z()));
	}
	static inline float clamp01(float v) {
		if (v > 1) v = 1;
		else if (v < 0) v = 0;
		return v;
	}

	static inline uint8_t gammaCorrection(float radiance) {
		return static_cast<uint8_t>(255.f * clamp01(powf(radiance, 1.f / 2.2f)));
	}

	static inline float radians(float x) { 
        return x * PI / 180; 
    }

	static inline Vec3f deNan(const Vec3f& vec, float val = 0) {
		Vec3f tmp = vec;
		if (vec.x() != vec.x()) tmp.x() = val;
		if (vec.y() != vec.y()) tmp.y() = val;
		if (vec.z() != vec.z()) tmp.z() = val;
		return tmp;
	}

	static inline Vec3f min(const Vec3f& a, const Vec3f& b) {
		return Vec3f(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
	}
	static inline Vec3f min(const Vec3f& a, const Vec3f& b, const Vec3f& c) {
		return min(min(a, b), c);
	}
	static inline Vec3f max(const Vec3f& a, const Vec3f& b) {
		return Vec3f(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
	}
	static inline Vec3f max(const Vec3f& a, const Vec3f& b, const Vec3f& c) {
		return max(max(a, b), c);
	}

}

// Random Sampler Part
#include <random>
class RandomSampler {
public:
	RandomSampler() = default;
	float get1D() { 
        return dis(engine); 
    }
	Vec2f get2D() { 
        return { 
            dis(engine), dis(engine) 
        }; 
    }
	void setSeed(int i) { 
        engine.seed(i); 
    }
private:
	std::default_random_engine engine;
	std::uniform_real_distribution<float> dis;
};

// Declare the class

class BSDF;
class Geometry;
class AABB;

#endif /* UTILS_HPP_ */