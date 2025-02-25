#pragma once
#include <memory>

#include "mathlib.h"
#include "pdf.h"

namespace RayTracer {
class Ray;
class HitResult;
enum class MaterialType {
    Lambert,
    Metal,
    Dielectric,
    DiffuseLight,
    Isotropic,
    None
};
struct ScatterResult {
    MaterialType mat_type;
    Vector3 albedo;
    bool skip_pdf;
    std::shared_ptr<Pdf> pdf;
    Ray scatter_ray;
};

class Material {
   public:
    Material() {}
    virtual ~Material() = default;

    virtual Vector3 Emitted(const Ray& inRay, const HitResult& hit) const {
        return Vector3(0, 0, 0);
    }

    virtual float ScatteringPDF(const Ray& inRay, const HitResult& hit,
                                const Ray& scatterRay) const {
        return 0.0f;
    }

    /// @brief
    /// @param inRay:       The ray that hit the material
    /// @param hit:         The hit result of the ray
    /// @param albedo: The albedo of the material
    /// @param scatterRay:  The ray that is scattered by the material
    /// @return
    virtual bool Scatter(const Ray& inRay, const HitResult& hit,
                         ScatterResult& scatter) const = 0;
};
}  // namespace RayTracer