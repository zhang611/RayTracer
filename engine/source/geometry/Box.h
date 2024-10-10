#pragma once

#include "Geometry.h"
#include "ray.h"
#include <cmath>
#include "mathlib.h"
#include <memory>
#include "Bounds.h"
#include "Quad.h"
#include "scene/Scene.h"


namespace RayTracer
{
    class Box : public Scene
    {
    public:
        Box(Vector3 center, Vector3 size, std::shared_ptr<Material> mat)
        {
            // generate 6 quads
            //front
            Vector3 leftbottom = center - size / 2;
            Vector3 righttop = center + size / 2;
            auto front = std::make_shared<Quad>(leftbottom, Vector3::right * size.x, Vector3::up * size.y, mat);
            auto back = std::make_shared<Quad>(righttop, -Vector3::right * size.x, -Vector3::up * size.y, mat);
            auto left = std::make_shared<Quad>(leftbottom, Vector3::forward * size.z, Vector3::up * size.y, mat);
            auto right = std::make_shared<Quad>(righttop, -Vector3::forward * size.z, -Vector3::up * size.y, mat);
            auto top = std::make_shared<Quad>(righttop, -Vector3::right * size.x, -Vector3::forward * size.z, mat);
            auto bottom = std::make_shared<Quad>(leftbottom, Vector3::right * size.x, Vector3::forward * size.z, mat);

            // sides.push_back(front);
            // sides.push_back(back);
            // sides.push_back(left);
            // sides.push_back(right);
            // sides.push_back(top);
            // sides.push_back(bottom);
            add(front);
            add(back);
            add(left);
            add(right);
            add(top);
            add(bottom);

            // bounds = Bounds(center, size);
            BuildBVH();

        }

        ~Box() override {}


        // using Hittable::Hit;
        // bool Hit(const Ray& r, Interval t_interval, HitResult& res) const override
        // {
        //     HitResult result;
        //     bool hit_anything = false;
        //     auto closest_so_far = t_interval.max;

        //     for (const auto& side : sides)
        //     {
        //         if (side->Hit(r, Interval(t_interval.min, closest_so_far), result))
        //         {
        //             hit_anything = true;
        //             closest_so_far = result.t;
        //             res = result;
        //         }
        //     }
        //     return hit_anything;
        // }

        // Bounds GetAABB() const override
        // {
        //     return bounds;
        // }
        
    private:
        // std::vector<std::shared_ptr<Hittable>> sides;
        // Bounds bounds;
    };
}