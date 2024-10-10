#pragma once
#include "Geometry.h"
#include "ray.h"
#include <cmath>
#include "mathlib.h"
#include <memory>
#include "Bounds.h"
#include "Lights.h"


namespace RayTracer
{
    class Triangle : public Hittable
    {
    public:
        Triangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, std::shared_ptr<Material> m)
            : v0(_p0), v1(_p1), v2(_p2), mat(m)
        {
            e1 = v1 - v0;
            e2 = v2 - v0;
            n = e1.crossProduct(e2);
            
            areaSize = n.length();
            n.normalize();

            D = n.dot(v0);

            auto min = Vector3::Min(v0, Vector3::Min(v1, v2));
            auto max = Vector3::Max(v0, Vector3::Max(v1, v2));
            bounds = Bounds();
            bounds.SetMinMax(min, max);
            bounds.Padding();
        }

        ~Triangle() override {}

        using Hittable::Hit;
        bool Hit(const Ray& r, Interval t_interval, HitResult& res) const override
        {
            auto denom = n.dot(r.direction());
            if(fabsf(denom) < kEpsilon) return false;
            
            float t = (D - n.dot(r.origin())) / denom;
            if(!t_interval.Contains(t)) return false;

            auto hitpoint = r.at(t);
            bool isInTriangle = IsInTriangle(hitpoint);
            if(!isInTriangle) return false;

            res.t = t;
            res.hit_point = hitpoint;
            res.set_face_normal(r, n);
            res.hit_material = mat.get();
            res.uv = Vector2(0.0f, 0.0f);   // TODO: calculate uv
            return true;
        }

        float pdfValue(const Vector3& origin, const Vector3& dir) const override 
        { 
            return 0.0f;
        }

        Vector3 random(const Vector3& origin) const override 
        { 
            return Vector3::zero;
        }

        Bounds GetAABB() const override
        {
            return bounds;
        }

        // left hand coordinate
        Vector3 normal() const { return n; }


        bool IsInTriangle(const Vector3 p) const
        {
            auto v0p = p - v0;
            auto v1p = p - v1;
            auto v2p = p - v2;

            auto c0 = v0p.crossProduct(v1p);
            auto c1 = v1p.crossProduct(v2p);
            auto c2 = v2p.crossProduct(v0p);

            return (c0.dot(c1) >= 0.0f) && (c1.dot(c2) >= 0.0f);
        }

    private:
        Vector3 v0, v1, v2;
        Vector3 e1, e2;

        Vector3 n;

        float D;    //  Ax + By + Cz = D --> n(x, y, z) = D

        std::shared_ptr<Material> mat;
        Bounds bounds;
        float areaSize;
    };
}