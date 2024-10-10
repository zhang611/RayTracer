#include "Bounds.h"
#include "ray.h"

namespace RayTracer
{
    bool Bounds::Intersects(const Bounds bounds)
    {
        auto min = GetMin();
        auto max = GetMax();
        auto boundsMin = bounds.GetMin();
        auto boundsMax = bounds.GetMax();
        return  min.x <= boundsMax.x && max.x >= boundsMin.x && 
                min.y <= boundsMax.y && max.y >= boundsMin.y && 
                min.z <= boundsMax.z && max.z >= boundsMin.z;
    }

    
    bool Bounds::IntersectRay(Ray ray, Interval& ray_t) const
    {
        auto boundsMin = GetMin();
        auto boundsMax = GetMax();
        auto ray_dir = ray.direction();
        auto ray_origin = ray.origin();
        auto invdir = Vector3(1.0f / ray_dir.x, 1.0f / ray_dir.y, 1.0f / ray_dir.z);

        float t0, t1;
        for(int i = 0 ; i < 3; ++i) //x, y ,z axis
        {
            t0 = (boundsMin[i] - ray_origin[i]) * invdir[i];
            t1 = (boundsMax[i] - ray_origin[i]) * invdir[i];
            if(t0 > t1) std::swap(t0, t1);
            ray_t.min = t0 > ray_t.min ? t0 : ray_t.min;
            ray_t.max = t1 < ray_t.max ? t1 : ray_t.max;
            if(ray_t.min > ray_t.max)
                return false;
        }

        return true;
    }
    
    int Bounds::MaxExtent() const
    {
        if(m_Extents.x > m_Extents.y && m_Extents.x > m_Extents.z)
            return 0;
        else if(m_Extents.y > m_Extents.z)
            return 1;
        else
            return 2;
    }

    void Bounds::Encapsulate(Vector3 point)
    {
        SetMinMax(Vector3::Min(GetMin(), point), Vector3::Max(GetMax(), point));
    }

    void Bounds::Encapsulate(Bounds bounds)
    {
        Encapsulate(bounds.m_Center - bounds.m_Extents);
        Encapsulate(bounds.m_Center + bounds.m_Extents);
    }

    void Bounds::Expand(float amount)
    {
        amount *= 0.5f;
        m_Extents += Vector3(amount, amount, amount);
    }

    void Bounds::Expand(Vector3 amount)
    {
        m_Extents += amount * 0.5f;
    }
    
    void Bounds::Padding(float amount)
    {
        if(m_Extents.x < amount) m_Extents.x += amount;
        if(m_Extents.y < amount) m_Extents.y += amount;
        if(m_Extents.z < amount) m_Extents.z += amount;
    }

    bool Bounds::Contains(Vector3 point)
    {
        auto min = GetMin();
        auto max = GetMax();
        return  min.x <= point.x && max.x >= point.x &&
                min.y <= point.y && max.y >= point.y &&
                min.z <= point.z && max.z >= point.z;
    }

    float Bounds::SqrDistance(Vector3 point)
    {
        auto min = GetMin();
        auto max = GetMax();
        float sqrDistance = 0.0f;
        float delta = 0.0f;
        if(point.x < min.x)
        {
            delta = point.x - min.x;
            sqrDistance += delta * delta;
        }
        else if(point.x > max.x)
        {
            delta = point.x - max.x;
            sqrDistance += delta * delta;
        }
        if(point.y < min.y)
        {
            delta = point.y - min.y;
            sqrDistance += delta * delta;
        }
        else if(point.y > max.y)
        {
            delta = point.y - max.y;
            sqrDistance += delta * delta;
        }
        if(point.z < min.z)
        {
            delta = point.z - min.z;
            sqrDistance += delta * delta;
        }
        else if(point.z > max.z)
        {
            delta = point.z - max.z;
            sqrDistance += delta * delta;
        }
        return sqrDistance;
    }
    Vector3 Bounds::ClosestPoint(Vector3 point)
    {
        auto min = GetMin();
        auto max = GetMax();
        Vector3 closestPoint = point;
        if(point.x < min.x)
            closestPoint.x = min.x;
        else if(point.x > max.x)
            closestPoint.x = max.x;
        if(point.y < min.y)
            closestPoint.y = min.y;
        else if(point.y > max.y)
            closestPoint.y = max.y;
        if(point.z < min.z)
            closestPoint.z = min.z;
        else if(point.z > max.z)
            closestPoint.z = max.z;
        return closestPoint;
    }
}