#pragma once
#include "Vector.h"
#include <cmath>

struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    Color(): r(0), g(0), b(0), a(255) {}
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255): r(r), g(g), b(b), a(a) {}
    Color(const Color& c): r(c.r), g(c.g), b(c.b), a(c.a) {}
    Color(Color&& c): r(c.r), g(c.g), b(c.b), a(c.a) {}

    Color& operator=(const Color& c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
        a = c.a;
        return *this;
    }

    Color operator+(const Color& c) const
    {
        return Color( static_cast<unsigned char>(r + c.r), 
                      static_cast<unsigned char>(g + c.g), 
                      static_cast<unsigned char>(b + c.b), 
                      static_cast<unsigned char>(a + c.a) );
    }

    Color& operator+=(const Color& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        a += c.a;
        return *this;
    }

    Color operator*(const Color& c) const
    {
        return Color( static_cast<unsigned char>(r * c.r), 
                      static_cast<unsigned char>(g * c.g), 
                      static_cast<unsigned char>(b * c.b), 
                      static_cast<unsigned char>(a * c.a) );
    }

    friend Color operator*(float f, const Color& c)
    {
        return Color( static_cast<unsigned char>(f * c.r), 
                      static_cast<unsigned char>(f * c.g), 
                      static_cast<unsigned char>(f * c.b), 
                      static_cast<unsigned char>(f * c.a) );
    }
    
    Vector3 ToVector3() const
    {
        return Vector3(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f);
    }

    static Color red;
    static Color green;
    static Color blue;
    static Color white;
    static Color black;
    static Color yellow;
    static Color cyan;
    static Color magenta;
    static Color gray;
    static Color orange;
    static Color purple;
    static Color brown;
    static Color pink;

    static Color FromVector3(const Vector3& v, bool gamma_correct = true)
    {
        Vector3 result = Vector3::Clamp(v, 0.0f, 1.0f);
        if(gamma_correct)
        {
            result = Vector3( sqrtf(result.x), sqrtf(result.y), sqrtf(result.z) );
        }
        
        return Color( static_cast<unsigned char>(result.x * 255), 
                      static_cast<unsigned char>(result.y * 255), 
                      static_cast<unsigned char>(result.z * 255) );
    }

};

