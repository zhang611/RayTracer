#pragma once
#include "Hittable.h"
#include "Bounds.h"
#include <vector>

namespace RayTracer
{
    class Scene;
    class BVHAccel;

    class BVHNode : public Hittable
    {
        friend class BVHAccel;
        public:
            BVHNode(std::shared_ptr<Hittable> left, std::shared_ptr<Hittable> right = nullptr);
            ~BVHNode() override {}

            using Hittable::Hit;
            bool Hit(const Ray& r, Interval t_interval, HitResult& output_result) const override;

            Bounds GetAABB() const override { return bounds; }

        private:
            

            std::shared_ptr<Hittable> left{nullptr};
            std::shared_ptr<Hittable> right{nullptr};
            Bounds bounds;
    };

    class BVHAccel
    {
    public:
        enum class SplitMethod { NAIVE = 0, SAH = 1 };
        static std::shared_ptr<BVHNode> Build(const Scene& scene, SplitMethod splitMethod = SplitMethod::NAIVE);
        static std::shared_ptr<BVHNode> recursiveBuild(std::vector<std::shared_ptr<Hittable>>& objects, SplitMethod splitMethod);
    };
}