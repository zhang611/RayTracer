#pragma once
#include "Hittable.h"

namespace RayTracer
{
    class Ray;
    class Geometry : public Hittable
    {
    public:
        using Hittable::Hittable;
        virtual ~Geometry() = default;

    protected:
    };
}