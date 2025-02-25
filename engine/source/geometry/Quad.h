#pragma once
#include <cmath>
#include <memory>

#include "Bounds.h"
#include "Geometry.h"
#include "Lights.h"
#include "mathlib.h"
#include "ray.h"

namespace RayTracer {
class Quad : public Hittable {
   public:
    Quad(const Vector3& _Q, const Vector3& _u, const Vector3& _v,
         std::shared_ptr<Material> m)
        : Q(_Q), u(_u), v(_v), mat(m) {
        auto center = Q + (u + v) * 0.5f;
        bounds = Bounds(center, Vector3::Abs(u + v));
        bounds.Padding();

        n = u.crossProduct(v);
        w = n / n.dot(n);

        areaSize = n.length();

        n.normalize();
        D = n.dot(Q);
    }

    ~Quad() override {}

    using Hittable::Hit;
    bool Hit(const Ray& r, Interval t_interval, HitResult& res) const override {
        auto denom = n.dot(r.direction());
        if (fabsf(denom) < kEpsilon) return false;

        float t = (D - n.dot(r.origin())) / denom;
        if (!t_interval.Contains(t)) return false;

        auto hitpoint = r.at(t);
        auto uv = GetUV(hitpoint - Q);
        if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
            return false;

        res.t = t;
        res.hit_point = hitpoint;
        res.set_face_normal(r, n);
        res.hit_material = mat.get();
        res.uv = uv;
        return true;
    }

    float pdfValue(const Vector3& origin, const Vector3& dir) const override {
        HitResult res;
        if (!this->Hit(Ray(origin, dir), Interval(0.001f, infinity), res))
            return 0.0f;

        auto distanceSqr = res.t * res.t * dir.lengthSqr();
        auto cosine = fabs(dir.dot(res.normal) / dir.length());
        if (cosine < 0.000001f) return 0.0f;

        return distanceSqr / (cosine * areaSize);
    }

    Vector3 random(const Vector3& origin) const override {
        auto p = Q + (random01() * u) + (random01() * v);
        return p - origin;
    }

    Bounds GetAABB() const override { return bounds; }

    // left hand coordinate
    Vector3 normal() const { return n; }

    Vector2 GetUV(const Vector3& p) const {
        auto alpha = w.dot(p.crossProduct(v));
        auto beta = w.dot(u.crossProduct(p));
        return Vector2(alpha, beta);
    }

   private:
    Vector3 Q;
    Vector3 u;
    Vector3 v;

    // Plane
    Vector3 n;
    float D;
    //
    Vector3 w;

    std::shared_ptr<Material> mat;
    Bounds bounds;

    float areaSize;
};
}  // namespace RayTracer