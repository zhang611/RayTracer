#include "BVH.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include "scene/Scene.h"

namespace RayTracer
{
    inline void Partition_NAIVE(std::vector<std::shared_ptr<Hittable>>& objects, 
            std::vector<std::shared_ptr<Hittable>>& left, std::vector<std::shared_ptr<Hittable>>& right)
    {
        int objs_count = objects.size();
        Bounds centroidBounds = objects[0]->GetAABB();
        for (int i = 0; i < objs_count; ++i)
            centroidBounds.Encapsulate(objects[i]->GetAABB().GetCenter());
        
        int dim = centroidBounds.MaxExtent();

        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->GetAABB().GetCenter().x <
                    f2->GetAABB().GetCenter().x;
            });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->GetAABB().GetCenter().y <
                    f2->GetAABB().GetCenter().y;
            });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->GetAABB().GetCenter().z <
                    f2->GetAABB().GetCenter().z;
            });
            break;
        }
        auto beginning = objects.begin();
        auto middling = objects.begin() + (objs_count / 2);
        auto ending = objects.end();

        left = std::vector<std::shared_ptr<Hittable>>(beginning, middling);
        right = std::vector<std::shared_ptr<Hittable>>(middling, ending);
    }
    std::shared_ptr<BVHNode> BVHAccel::Build(const Scene& scene, SplitMethod splitMethod)
    {
        if(scene.objects.size() == 0)
            return nullptr;
        time_t start, stop;
        time(&start);
        auto objects = scene.objects;
        auto root = recursiveBuild(objects, splitMethod);

        time(&stop);
        double diff = difftime(stop, start);
        int hrs = (int)diff / 3600;
        int mins = ((int)diff / 60) - (hrs * 60);
        int secs = (int)diff - (hrs * 3600) - (mins * 60);

        std::cout << "BVH Generation took " << hrs << " hours, " << mins << " minutes, " << secs << " seconds\n";
        return root;
    }

    std::shared_ptr<BVHNode> BVHAccel::recursiveBuild(std::vector<std::shared_ptr<Hittable>>& objects, SplitMethod splitMethod)
    {
        std::shared_ptr<BVHNode> node = nullptr;
        Bounds bounds;
        int objs_count = objects.size();
        bounds = objects[0]->GetAABB();
        for (int i = 1; i < objs_count; ++i)
            bounds.Encapsulate(objects[i]->GetAABB());
        if(objs_count == 1)
        {
            node = std::make_shared<BVHNode>(objects[0]);
        }else if(objs_count == 2)
        {
            node = std::make_shared<BVHNode>(objects[0], objects[1]);
        }else
        {
            std::vector<std::shared_ptr<Hittable>> leftPrimitives;
            std::vector<std::shared_ptr<Hittable>> rightPrimitives;
            switch (splitMethod)
            {
            case SplitMethod::SAH:
                throw std::runtime_error("SplitMethod::SAH not implemented");
                break;
            default:
                Partition_NAIVE(objects, leftPrimitives, rightPrimitives);
                break;
            }

            assert(objs_count == (leftPrimitives.size() + rightPrimitives.size()));

            node = std::make_shared<BVHNode>(
                recursiveBuild(leftPrimitives, splitMethod), 
                recursiveBuild(rightPrimitives, splitMethod));
        }

        return node;
    }

    BVHNode::BVHNode(std::shared_ptr<Hittable> left, std::shared_ptr<Hittable> right):
        left(left), right(right)
    {
        bounds = left->GetAABB();
        if(right) bounds.Encapsulate(right->GetAABB());
    }


    bool BVHNode::Hit(const Ray& ray, Interval t_interval, HitResult& output_result) const
    {
        if(!bounds.IntersectRay(ray, t_interval)) return false;

        bool hit_left = left->Hit(ray, t_interval, output_result);
        bool hit_right =right != nullptr? 
            right->Hit(ray, Interval(t_interval.min, hit_left ? output_result.t : t_interval.max), output_result) : false;
        
        return hit_left || hit_right;
    }



}