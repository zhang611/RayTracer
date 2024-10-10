#pragma once
#include "mathlib.h"
#include "Material.h"
#include "Hittable.h"
#include "ray.h"
#include "Texture.h"
#include <iostream>

namespace RayTracer
{
    class Lambert : public Material
    {
        public:
            Lambert(Vector3 a) : pTex(std::make_shared<SolidTexture>(a)) {}
            Lambert(std::shared_ptr<SpatialTexture> tex) : pTex(tex) {}

            virtual bool Scatter(const Ray& inRay, const HitResult& hit, ScatterResult& scatter) const override
            {
                scatter.albedo = pTex->Value(hit.uv, hit.hit_point);
                scatter.mat_type = MaterialType::Lambert;
                scatter.skip_pdf = false;
                scatter.pdf = std::make_shared<CosinePdf>(hit.normal);
                // scatter.scatter_ray = Ray(hit.hit_point, Vector3::RandomOnHemisphere(hit.normal));
                return true;
            }

            float ScatteringPDF(const Ray& inRay, const HitResult& hit, const Ray& scatterRay) const override { 
                float cosine = hit.normal.dot(scatterRay.direction().normalized());
                return cosine < 0 ? 0 : cosine / PI;
            }

        private:
            Vector3 albedoColor;
            std::shared_ptr<SpatialTexture> pTex;
    };
}