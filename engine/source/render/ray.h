#pragma once

#include "Vector.h"

namespace RayTracer
{
    class Ray {
    public:
        Ray() {}
        Ray(const Vector3& origin, const Vector3& direction)
            : orig(origin), dir(direction), tm(0.0f) {}
        
        Ray(const Vector3& origin, const Vector3& direction, float time)
            : orig(origin), dir(direction), tm(time) {}
        
        Vector3 origin() const  { return orig; }
        Vector3 direction() const { return dir; }

        Vector3 at(float t) const {
            return orig + t*dir;
        }

        float time() const { return tm; }
    private:
        Vector3 orig;
        Vector3 dir;
        float tm;
    };
}