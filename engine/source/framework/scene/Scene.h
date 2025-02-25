#pragma once
#include <memory>
#include <vector>

#include "BVH.h"
#include "Bounds.h"
#include "Hittable.h"

namespace RayTracer {
class Scene : public Hittable {
   public:
    std::vector<std::shared_ptr<Hittable>> objects;

    Scene() {}
    Scene(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
        if (objects.size() == 1)
            bounds = object->GetAABB();
        else
            bounds.Encapsulate(object->GetAABB());
    }

    using Hittable::Hit;
    bool Hit(const Ray& r, Interval t_interval,
             HitResult& output_result) const override {
        if (m_root != nullptr) return m_root->Hit(r, t_interval, output_result);
        HitResult result;
        bool hit_anything = false;
        auto closest_so_far = t_interval.max;

        for (const auto& object : objects) {
            if (object->Hit(r, Interval(t_interval.min, closest_so_far),
                            result)) {
                hit_anything = true;
                closest_so_far = result.t;
                output_result = result;
            }
        }
        return hit_anything;
    }

    Bounds GetAABB() const override { return bounds; }

    void BuildBVH() { m_root = BVHAccel::Build(*this); }

   protected:
    Bounds bounds;
    std::shared_ptr<RayTracer::BVHNode> m_root{nullptr};
};
}  // namespace RayTracer