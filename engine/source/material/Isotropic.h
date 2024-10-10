#pragma once
#include "mathlib.h"
#include "Material.h"
#include "Hittable.h"
#include "ray.h"
#include "Texture.h"


namespace RayTracer
{
    class Isotropic : public Material
    {
        public:
            Isotropic(Vector3 a) : albedoTex(std::make_shared<SolidTexture>(a)) {}
            Isotropic(std::shared_ptr<SpatialTexture> tex) : albedoTex(tex) {}

            virtual bool Scatter(const Ray& inRay, const HitResult& hit, ScatterResult& scatter) const override
            {
                scatter.mat_type = MaterialType::Isotropic;
                scatter.albedo = albedoTex->Value(hit.uv, hit.hit_point);
                scatter.pdf = std::make_shared<SpherePdf>();
                scatter.skip_pdf = false;
                
                // scatterRay = Ray(hit.hit_point, Vector3::RandomUnitSphere());
                // albedo = albedoTex->Value(hit.uv, hit.hit_point);
                // pdf = 1.0f / (4.0f * PI);
                return true;
            }
            
            float ScatteringPDF(const Ray& inRay, const HitResult& hit, const Ray& scatterRay) const override { 
                
                return 1.0f / (4.0f * PI);
            }
        private:
            std::shared_ptr<SpatialTexture> albedoTex;
    };
}