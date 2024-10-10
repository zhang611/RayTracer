#pragma once
#include "mathlib.h"
#include "Material.h"
#include "Hittable.h"
#include "ray.h"

namespace RayTracer
{
    class Dielectric : public Material
    {
        public:
            Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

            virtual bool Scatter(const Ray& inRay, const HitResult& hit, ScatterResult& scatter) const override
            {
                scatter.mat_type = MaterialType::Dielectric;
                scatter.albedo = Vector3(1.0f, 1.0f, 1.0f);
                scatter.pdf = nullptr;
                scatter.skip_pdf = true;


                Vector3 unit_dir = inRay.direction().normalized();
                float refraction_ratio = hit.front_face ? (1.0f/ir) : ir;
                float cos_theta = fminf(1.0f, -unit_dir.dot(hit.normal));
                float sin_theta = sqrtf(1.0f - cos_theta*cos_theta);
                Vector3 direction = hit.normal;
                bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
                if ( cannot_refract || reflectance(cos_theta, refraction_ratio) > random01()) {
                    direction = unit_dir.reflect(hit.normal);
                } else {
                    direction = unit_dir.refract(hit.normal, refraction_ratio);
                }
                scatter.scatter_ray = Ray(hit.hit_point, direction);
                return true;
            }
            
        private:
            float ir; // Index of Refraction

        static float reflectance(float cosine, float ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*powf((1 - cosine),5);
        }
    };
}