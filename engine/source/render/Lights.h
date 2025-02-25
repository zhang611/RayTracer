#pragma once
#include <memory>
#include <vector>

#include "Hittable.h"
#include "mathlib.h"
#include "scene/Scene.h"

namespace RayTracer {
class Lights : public Scene {
   public:
    static std::shared_ptr<Lights> lights;

   public:
    float pdfValue(const Vector3& origin, const Vector3& dir) const override {
        auto weight = 1.0f / objects.size();
        auto sum = 0.0f;

        for (const auto& object : objects)
            sum += weight * object->pdfValue(origin, dir);

        return sum;
    }

    Vector3 random(const Vector3& origin) const override {
        auto int_size = static_cast<int>(objects.size());
        return objects[std::clamp(random_int(0, int_size - 1), 0, int_size - 1)]
            ->random(origin);
    }
};

}  // namespace RayTracer