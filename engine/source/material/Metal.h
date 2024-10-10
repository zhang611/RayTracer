#pragma once
#include "mathlib.h"
#include "Material.h"
#include "Hittable.h"
#include "ray.h"

namespace RayTracer
{
    class Metal : public Material
    {
        public:
            Metal(const Vector3& a, float fuzz = 0.0f) : albedo(a),fuzz(fuzz) {}

            virtual bool Scatter(const Ray& inRay, const HitResult& hit, ScatterResult& scatter) const override
            {
                scatter.mat_type = MaterialType::Metal;
                scatter.albedo = albedo;
                scatter.pdf = nullptr;
                scatter.skip_pdf = true;
                Vector3 reflect = inRay.direction().reflect(hit.normal).normalized();
                scatter.scatter_ray = Ray(hit.hit_point, reflect + fuzz * Vector3::RandomUnitSphere());
                return true;
            }
            
        private:
            Vector3 albedo;
            float fuzz;
    };
}