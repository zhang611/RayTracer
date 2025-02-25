#pragma once
#include <cmath>
#include <iostream>
#include <memory>

#include "Bounds.h"
#include "Geometry.h"
#include "Isotropic.h"
#include "mathlib.h"
#include "ray.h"

namespace RayTracer {
class ConstantMedium : public Hittable {
   public:
    ConstantMedium(std::shared_ptr<Hittable> b, float density, Vector3 color)
        : boundary(b),
          neg_inv_density(-1 / density),
          phase_function(std::make_shared<Isotropic>(color)) {}

    // ConstantMedium(std::shared_ptr<Hittable> b, float density,
    // std::shared_ptr<Material>& mat) : boundary(b),
    // neg_inv_density(-1/density),
    // phase_function(std::make_shared<Isotropic>(mat))
    // {
    // }

    using Hittable::Hit;
    bool Hit(const Ray& r, Interval t_interval, HitResult& res) const override {
        // Print occasional samples when debugging. To enable, set enableDebug
        // true.
        const bool enableDebug = false;
        const bool debugging = enableDebug && random01() < 0.00001f;

        HitResult res1, res2;
        if (!boundary->Hit(r, Interval::universe, res1)) return false;

        if (!boundary->Hit(r, Interval(res1.t + 0.0001f, infinity), res2))
            return false;

        if (res1.t < t_interval.min) res1.t = t_interval.min;
        if (res2.t > t_interval.max) res2.t = t_interval.max;

        if (res1.t >= res2.t) return false;

        auto ray_length = r.direction().length();
        auto distance_inside_boundary = (res2.t - res1.t) * ray_length;
        auto hit_distance = neg_inv_density * log(random01());

        if (hit_distance > distance_inside_boundary) return false;

        res.t = res1.t + hit_distance / ray_length;
        res.hit_point = r.at(res.t);

        if (debugging) {
            std::clog << "hit_distance = " << hit_distance << '\n'
                      << "rec.t = " << res.t << '\n'
                      << "rec.p = " << res.hit_point << '\n';
        }

        res.normal = Vector3(1.0f, 0.0f, 0.0f);  // arbitrary
        res.front_face = true;                   // also arbitrary
        res.hit_material = phase_function.get();

        return true;
    }
    virtual Bounds GetAABB() const { return boundary->GetAABB(); }

   private:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phase_function;
    float neg_inv_density;
};
}  // namespace RayTracer