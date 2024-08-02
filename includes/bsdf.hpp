#ifndef BSDF_HPP_
#define BSDF_HPP_

#include "interaction.hpp"
#include "configs.hpp"

class BSDF {
public:
    BSDF() = default;
    BSDF(const Vec3f& color): color(color) {}
    BSDF(const MaterialConfig& config): color(config.color) {}
    virtual ~BSDF() = default;

    [[nodiscard]] virtual Vec3f evaluate(Interaction& interaction) const {
        return getPDF(interaction) * color;
    }
    virtual float sample(Interaction& interaction, RandomSampler& sampler) const = 0;
protected:
    virtual float getPDF(Interaction& interaction) const = 0;
    [[nodisgard]] virtual bool isDelta() const = 0;

    // Any BSDF should have a color
    Vec3f color;
};

class IdealDiffuseBSDF: public BSDF {
public:
    IdealDiffuseBSDF(const Vec3f& color): BSDF(color) {}
    IdealDiffuseBSDF(const MaterialConfig& config): BSDF(config) {}
    virtual float sample(Interaction& interaction, RandomSampler& sampler) const override;
    
protected:
    virtual float getPDF(Interaction& interaction) const override {
        return 1.0f / (2 * PI);
    }
    virtual bool isDelta() const override {
        return false;
    }
};

class IdealSpecularBSDF: public BSDF {
public:
    IdealSpecularBSDF(const Vec3f& color): BSDF(color) {}
    IdealSpecularBSDF(const MaterialConfig& config): BSDF(config) {}
    virtual float sample(Interaction& interaction, RandomSampler& sampler) const override;

protected:
    virtual float getPDF(Interaction& interaction) const override {
        Vec3f normal = interaction.normal.normalized(),
            wi = interaction.w_i.normalized(), wo = interaction.w_o.normalized();
        // Generate the reflect direction
        Vec3f reflect_dir = 2 * (normal.dot(wo)) * normal - wo;
        // Check the angle between the reflect direction and the outgoing direction
        if ((reflect_dir - wi).norm() < EPS) {
            return 1.0f; // Delta BSDF
        }
        else return 0.0f;
    }
    virtual bool isDelta() const override {
        return true;
    }
};

#endif // BSDF_HPP_