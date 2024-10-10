#pragma once
#include "mathlib.h"
#include "Material.h"
#include "Hittable.h"
#include "ray.h"
#include "Texture.h"

namespace RayTracer
{
    class DiffuseLight : public Material
    {
    public:
        DiffuseLight(std::shared_ptr<SpatialTexture> tex) : emit(tex) {}
        DiffuseLight(Vector3 c) : emit(std::make_shared<SolidTexture>(c)) {}

        bool Scatter(const Ray &inRay, const HitResult &hit, ScatterResult& scatter) const override
        {
            return false;
        }

        Vector3 Emitted(const Ray &inRay, const HitResult &hit) const override
        {
            if (!hit.front_face)
                return Vector3(0.0f, 0.0f, 0.0f);
            return emit->Value(hit.uv, hit.hit_point);
        }

    private:
        std::shared_ptr<SpatialTexture> emit;
    };
}