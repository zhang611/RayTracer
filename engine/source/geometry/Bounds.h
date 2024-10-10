#pragma once
#include "mathlib.h"

namespace RayTracer
{
    class Ray;
    struct Bounds
    {
    public:
        Bounds():
            m_Center(Vector3::zero),
            m_Extents(Vector3::zero)
        {
        }
        Bounds(Vector3 center, Vector3 size)
        {
            m_Center = center;
            m_Extents = size * 0.5f;
        }

        Bounds(const Bounds& a, const Bounds& b)
        {
            auto min = Vector3::Min(a.GetMin(), b.GetMin());
            auto max = Vector3::Max(a.GetMax(), b.GetMax());
            m_Center = (min + max) * 0.5f;
            m_Extents = (max - min) * 0.5f;
        }

        Bounds(Bounds const& other)
        {
            m_Center = other.m_Center;
            m_Extents = other.m_Extents;
        }
        Bounds& operator=(Bounds& other) noexcept
        {
            m_Center = other.m_Center;
            m_Extents = other.m_Extents;
            return *this;
        }
        ~Bounds() = default;

        Bounds operator+(const Vector3& offset) const { return Bounds(m_Center + offset, m_Extents*2); }
        friend Bounds operator+(const Vector3& offset, const Bounds& bounds) {return bounds + offset; }
        bool operator==(const Bounds& other) const
        {
            return m_Center == other.m_Center && m_Extents == other.m_Extents;
        }
        bool operator!=(const Bounds& other) const
        {
            return !(*this == other);
        }

        bool Intersects(const Bounds bounds);
        bool IntersectRay(Ray ray, Interval& ray_t) const;
        // bool IntersectRay(Ray ray, float &distance);
        
        int MaxExtent() const;
        void Encapsulate(Vector3 point);
        void Encapsulate(Bounds bounds);
        void Expand(float amount);
        void Expand(Vector3 amount);
        void Padding(float amount = 0.001f);
        bool Contains(Vector3 point);
        float SqrDistance(Vector3 point);
        Vector3 ClosestPoint(Vector3 point);
        Vector3 GetCenter() { return m_Center;  }
        void SetCenter(Vector3 center)  { m_Center = center; }

        Vector3 GetExtents() { return m_Extents; }
        void SetExtents(Vector3 extents) { m_Extents = extents; }

        Vector3 GetSize() { return m_Extents * 2.f; }
        void SetSize(Vector3 size) { m_Extents = size * 0.5f; }

        inline Vector3 GetMin() const { return m_Center - m_Extents; }
        inline void SetMin(Vector3 min) { SetMinMax(min, GetMax()); }

        inline Vector3 GetMax() const { return m_Center + m_Extents; }
        inline void SetMax(Vector3 max) { SetMinMax(GetMin(), max); }
        void SetMinMax(Vector3 min, Vector3 max) { m_Extents = (max - min) * 0.5f; m_Center = min + m_Extents; }
        
    private:
        Vector3 m_Center;
        Vector3 m_Extents;
    };
}