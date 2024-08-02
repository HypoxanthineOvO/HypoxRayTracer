#include "bsdf.hpp"

float IdealDiffuseBSDF::sample(Interaction& interaction, RandomSampler& sampler) const {
    // Generate a random direction
    float theta = sampler.get1D(), phi = sampler.get1D();
    float x = cosf(2 * PI * theta) * sqrtf(phi),
        y = sinf(2 * PI * theta) * sqrtf(phi),
        z = sqrtf(1 - phi);
    Vec3f new_wi = Vec3f(x, y, z);
    Mat3f T = Eigen::Quaternionf::FromTwoVectors(Vec3f(0, 0, 1), interaction.normal.normalized()).toRotationMatrix();
    new_wi = (T * new_wi).normalized();

    interaction.w_i = new_wi;

    // Return the probability
    float pdf = sqrtf(1 - phi) / PI;
    return pdf;
}

float IdealSpecularBSDF::sample(Interaction& interaction, RandomSampler& sampler) const {
    // The fixed reflect direction
    Vec3f normal = interaction.normal.normalized(),
        wi = interaction.w_i.normalized(), wo = interaction.w_o.normalized();
    // Generate the reflect direction
    Vec3f reflect_dir = 2 * (normal.dot(wo)) * normal - wo;
    
    interaction.w_i = reflect_dir;

    return 1.0f;
}