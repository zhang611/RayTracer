#pragma once
#include <cmath>
#include <memory>

#include "Bounds.h"
#include "Geometry.h"
#include "ray.h"

namespace RayTracer {
class Sphere : public Hittable {
   public:
    Sphere(const Vector3 center, float radius,
           std::shared_ptr<Material> mat = nullptr)
        : m_center(center), m_radius(radius), material(mat) {}
    ~Sphere() override {}

    using Hittable::Hit;
    // bool Hit(const Ray& r, HitResult& rec) const override
    // {
    //     return Hit(r, 0.0f, 1000.0f, rec);
    // }

    bool Hit(const Ray &r, Interval t_interval,
             HitResult &result) const override {
        Vector3 oc = r.origin() - m_center;
        auto r_dot_r = r.direction().dot(r.direction());
        auto oc_dot_r = oc.dot(r.direction());
        auto oc_dot_oc = oc.dot(oc);
        auto a = r_dot_r;
        auto half_b = oc_dot_r;
        auto c = oc_dot_oc - m_radius * m_radius;
        auto discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;

        // Find the nearest root that lies in the acceptable range.
        result.t = (-half_b - sqrtf(discriminant)) / a;
        if (!t_interval.Surrounds(result.t)) {
            result.t = (-half_b + sqrtf(discriminant)) / a;
            if (!t_interval.Surrounds(result.t)) return false;
        }
        result.hit_point = r.at(result.t);
        result.normal = (result.hit_point - m_center) / m_radius;
        result.set_face_normal(r, result.normal);
        result.hit_material = material.get();
        result.uv = GetUV(result.normal);
        return true;
    }

    Bounds GetAABB() const override {
        float size = m_radius * 2;
        return Bounds(m_center, Vector3::one * size);
    }

    /**
     * @brief UV is calculated by the spherical coordinates
     *          <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
     * @param dir: the direction(normal) of the hit point, centerer at sphere
     * center
     * @return Vector2
     */
    Vector2 GetUV(Vector3 dir) const {
        float theta = acosf(-dir.y);  // dir.y : [-1, 1] , theta : [0, PI]
        float phi = atan2f(dir.z, dir.x) + PI;
        return Vector2(phi / (2.0f * PI), theta / PI);
    }

    /**
     * @brief
     *      we need get the pdf value of the ray direction,
     * @param origin    ray origin
     * @param dir       ray direction
     * @return float
     */
    float pdfValue(const Vector3 &origin, const Vector3 &dir) const {
        HitResult result;
        if (!Hit(Ray(origin, dir), Interval(0.001f, infinity), result))
            return 0.0f;

        float cos_theta_max =
            sqrtf(1.0f - m_radius * m_radius / (m_center - origin).lengthSqr());
        float solid_angle = 2.0f * PI * (1.0f - cos_theta_max);
        return 1.0f / solid_angle;
    }

    Vector3 random(const Vector3 &hit_point) const {
        Vector3 direction =
            m_center - hit_point;  // ray is from hit point to sphere
                                   // center(here sphere is a light)
        Vector3 local_dir = random_to_sphere(m_radius, direction.lengthSqr());
        Vector3 normal = direction.normalized();
        Vector3 tangent, bitangent;
        normal.tangentSpace(tangent, bitangent);
        return tangent * local_dir.x + bitangent * local_dir.y +
               normal * local_dir.z;
    }

    float radius() const { return m_radius; }
    Vector3 center() const { return m_center; }

   private:
    float m_radius;
    Vector3 m_center;
    std::shared_ptr<Material> material;

    static Vector3 random_to_sphere(float radius, float distanceSqr) {
        auto r1 = Random::Random01();
        auto r2 = Random::Random01();

        // z = cos(theta) = sqrt(1-radius*radius/distance_squared)?
        float cos_theta = sqrtf(1.0f - radius * radius / distanceSqr);
        auto z = 1.0f + r2 * (cos_theta - 1.0f);

        auto phi = 2.0f * PI * r1;
        auto x = cos(phi) * sqrtf(1.0f - z * z);
        auto y = sin(phi) * sqrtf(1.0f - z * z);

        return Vector3(x, y, z);
    }
};
}  // namespace RayTracer