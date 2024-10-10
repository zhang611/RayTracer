#pragma once
#include "Geometry.h"
#include "ray.h"
#include "mathlib.h"
#include <memory>
#include "Bounds.h"

namespace RayTracer
{
    class Translation : public Hittable
    {
        public:
            Translation(std::shared_ptr<Hittable> p, const Vector3& displacement)
            : object(p), offset(displacement)
            {
                bounds = object->GetAABB() + offset;
            }
            ~Translation() override {}

            using Hittable::Hit;
            bool Hit(const Ray& r, Interval t_interval, HitResult& res) const
            {
                Ray offset_r(r.origin() - offset, r.direction());
                // Determine where (if any) an intersection occurs along the offset ray
                if (!object->Hit(offset_r, t_interval, res))
                    return false;

                // Move the intersection point forwards by the offset
                res.hit_point += offset;
                return true;
            }

            Bounds GetAABB() const override
            {
                return bounds;
            }

            static std::shared_ptr<Hittable> Translate(std::shared_ptr<Hittable> p, const Vector3& offset)
            {
                return std::make_shared<Translation>(p, offset);
            }

        private:
        std::shared_ptr<Hittable> object;
        Vector3 offset;
        Bounds bounds;
    };

    class Rotation: public Hittable
    {
        public:
            Rotation(std::shared_ptr<Hittable> p, const Vector3& rotate)
            : object(p), rotation(rotate)
            {
                // get bounds of rotated bounds
                bounds = object->GetAABB();
                auto minOffset = bounds.GetMin() - bounds.GetCenter();
                auto maxOffset = bounds.GetMax() - bounds.GetCenter();
                rotMatrix = Matrix4x4::Rotation(rotation);
                rotMatrixInv = rotMatrix.Transpose();   // inverse of rotation matrix is its transpose
                auto rotMinOffset = (rotMatrix * Vector4(minOffset, 1.0f)).xyz();
                auto rotMaxOffset = (rotMatrix * Vector4(maxOffset, 1.0f)).xyz();

                bounds.SetMinMax(rotMinOffset + bounds.GetCenter(), rotMaxOffset + bounds.GetCenter());
            }

            ~Rotation() override {}


            using Hittable::Hit;
            bool Hit(const Ray& r, Interval t_interval, HitResult& res) const
            {
                // change ray to object space

                Ray rotated_ray = Ray((rotMatrixInv * Vector4(r.origin(), 1.0f)).xyz(), (rotMatrixInv * Vector4(r.direction(), 0.0f)).xyz());

                // Determine where (if any) an intersection occurs along the offset ray
                if (!object->Hit(rotated_ray, t_interval, res))
                    return false;

                // change intersection point back to world space
                res.hit_point  = (rotMatrix * Vector4(res.hit_point, 1.0f)).xyz();
                res.normal = (rotMatrix * Vector4(res.normal, 0.0f)).xyz();
                return true;
            }

            Bounds GetAABB() const override
            {
                return bounds;
            }
            
            static std::shared_ptr<Hittable> Rotate(std::shared_ptr<Hittable> p, const Vector3& rotate)
            {
                return std::make_shared<Rotation>(p, rotate);
            }
        private:
        std::shared_ptr<Hittable> object;
        Vector3 rotation;
        Bounds bounds;
        Matrix4x4 rotMatrix;
        Matrix4x4 rotMatrixInv;
    };
}