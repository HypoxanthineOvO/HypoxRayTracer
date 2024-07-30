#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <memory>

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


constexpr float RAY_DEFAULT_MIN = 1e-5;
constexpr float RAY_DEFAULT_MAX = 1e7;
constexpr float PI = 3.141592653579f;
constexpr float INV_PI = 0.31830988618379067154;
constexpr float EPS = 1e-5;

// Utils Part
namespace utils {

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

#endif /* UTILS_HPP_ */