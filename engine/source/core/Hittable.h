#pragma once
#include "Vector.h"
#include "ray.h"
#include "Interval.h"

namespace RayTracer
{
    class Material;
    struct Bounds;

    class HitResult {
    public:
        float t;
        Vector3 hit_point;
        Material* hit_material;
        Vector2 uv;
        
        Vector3 normal;
        bool front_face;

        void set_face_normal(const Ray& r, const Vector3& outward_normal) {
            front_face = r.direction().dot(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal; // we hope the normal is always face to the ray
        }
    };

    class Hittable {
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const Ray& r, HitResult& res) const { return Hit(r, Interval(0.01f, infinity), res); }
        virtual bool Hit(const Ray& r, Interval t_interval, HitResult& res) const = 0;
        
        virtual float pdfValue(const Vector3& origin, const Vector3& dir) const { return 0.0f; }
        virtual Vector3 random(const Vector3& origin) const { return Vector3(1.0f, 0.0f, 0.0f); }
        virtual Bounds GetAABB() const = 0;
    };
}