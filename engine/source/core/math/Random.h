#pragma once
#include "Vector.h"
#include "mathcommon.h"
#include <cmath>

class Random
{
public:
    static Vector3 RandomCosineDirection()
    {
        auto r1 = random01();
        auto r2 = random01();

        auto phi = 2.0f * PI * r1;
        auto x = cos(phi) * sqrt(r2);
        auto y = sin(phi) * sqrt(r2);
        auto z = sqrt(1 - r2);

        return Vector3(x, y, z);
    }

    static float Random01()
    {
        static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        static std::mt19937 generator;
        return distribution(generator);
    }
};