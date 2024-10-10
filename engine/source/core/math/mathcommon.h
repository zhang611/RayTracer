#pragma once
#include <limits>
#include <cstdlib>
#include <random>
#include "Vector.h"

const float infinity = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385f;
const static float kEpsilon = 1E-06f;
const static float kRad2Deg = 57.29578f;
const static float kDeg2Rad = 0.01745329f;
// Utility Functions

inline float DegreeToRadian(float degrees)
{
    return degrees * PI / 180.0f;
}

inline float RadianToDegree(float radians)
{
    return radians * 180.0f / PI;
}

// Utility Functions

inline float degrees_to_radians(float degrees)
{
    return degrees * PI / 180.0f;
}
/**
 * @brief  Returns a random real in [0,1).
 *
 * @return float
 */
inline float random01()
{
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random01();
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_float(static_cast<float>(min), static_cast<float>(max + 1)));
}

inline Vector3 random_unit_vector()
{
    Vector3 unit_sphere;
    while (true)
    {
        unit_sphere = Vector3::random(-1.0f, 1.0f);
        if (unit_sphere.length() < 1.0f)
            break;
    }

    return unit_sphere.normalized();
}

inline Vector3 random_on_hemisphere(const Vector3 &normal)
{
    Vector3 on_unit_sphere = random_unit_vector();
    if (Vector3::Dot(on_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
